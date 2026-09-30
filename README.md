# PINN–PINO Embedded Pipeline for Turbulent Pipe-Flow Monitoring

## Overview

This repository implements an embedded physics-informed neural-network pipeline for transient pressure/flow modelling and anomaly detection in a pressurized pipe.

The deployed system uses **two TFLite models**:

1. **2-input forward PINN** — generates the pressure/flow background field over normalized pipe position and time.
2. **206-input, 6-output anomaly classifier** — identifies `NONE`, `LEAK`, or `CONSTRICTION`, and estimates the anomaly location and class-specific severity.

The final visualization is an **anomaly-annotated pressure map**:

```text
                TWO PRESSURE SENSORS
                         |
                         v
               206 FEATURE BUILDER
                         |
                         v
             +----------------------+
             | 6-output classifier  |
             +----------------------+
                         |
                  anomaly result
                         |
                         v
Normalized grid     2-input PINN
   (xi,tau)              |
       |                 v
       +-----------> [h, q]
                         |
                         v
                Physical reconstruction
                         |
                         v
                 P(x,t) pressure map
                         |
                         v
                 anomaly annotation
                         |
                         v
                      DISPLAY
```

> **Important:** the pressure map is a **1D pipe space × time map**, i.e. \(P(x,t)\). It is not a 2D CFD pressure field \(P(x,y)\).

---

# 1. Deployed Models

## 1.1 2-Input Forward PINN

Source:

```text
leakpinn/pinn.py
```

Export script:

```text
scripts/05_export_onnx.py
```

Typical exported artifact:

```text
results/05_pinn_forward_fp32.onnx
results/05_tflite_model/<generated .tflite>
```

### Architecture

```text
Linear(2 -> 32)
Tanh
Linear(32 -> 32)
Tanh
Linear(32 -> 32)
Tanh
Linear(32 -> 2)
```

Total trainable parameters:

```text
2,274
```

### Input

```text
[xi, tau]
```

where

$$
\xi = \frac{x}{L}
$$

and

$$
\tau = \frac{a_{\mathrm{nom}}t}{L}.
$$

The input tensor is:

```text
float32 [1, 2]
```

### Output

The network returns:

```text
[h, q]
```

with:

$$
h = \frac{H-H_{ss}}{H_s}.
$$

For the fixed exported model:

$$
H_s = 10\ \mathrm{m}.
$$

Therefore:

$$
H(\xi,\tau)=H_{ss}(\xi)+10h(\xi,\tau).
$$

The second output is the dimensionless flow perturbation used by the model's fixed physical reconstruction.

The model **does not directly output pressure in pascals/bar**.

---

# 2. Physical Pressure Reconstruction

For the fixed single-pipe model, the steady head profile is:

$$
H_{ss}(\xi)
=
H_{2,0}
+
\frac{H_{1,0}-H_{2,0}}{1-\xi_1}(1-\xi).
$$

Hence:

$$
H(\xi,\tau)
=
H_{ss}(\xi)+H_s h(\xi,\tau).
$$

For water:

$$
P(\xi,\tau)=\rho g H(\xi,\tau).
$$

Pressure in bar is:

$$
P_{\mathrm{bar}}
=
\frac{\rho g H}{10^5}.
$$

The final pressure grid is stored as:

```text
P_grid[time_index][space_index]
```

or equivalently:

$$
P_{j,i}=P(\xi_i,\tau_j).
$$

---

# 3. 206-Input Anomaly Classifier

Source:

```text
leakpinn/classify_net.py
```

Feature definition:

```text
leakpinn/localize_data.py
leakpinn/classify_data.py
```

Exported model:

```text
results/anomaly_classifier_fp32.tflite
```

### Input

```text
float32 [1, 206]
```

The 206-element feature vector contains:

```text
0..99       100 resampled perturbation-head samples, sensor 1
100..199    100 resampled perturbation-head samples, sensor 2
200         log(phi)
201         H1_0 / 100
202         H2_0 / 100
203         log(B_nom)
204         log(qv0)
205         xi1
```

Thus:

$$
206=100+100+6.
$$

### Output

The classifier returns:

```text
[p_none, p_leak, p_constriction,
 xi_frac, kappa_leak, kappa_constriction]
```

Output indices:

```text
[0] p_none
[1] p_leak
[2] p_constriction
[3] xi_frac
[4] kappa_leak
[5] kappa_constriction
```

The anomaly class is selected using:

$$
\mathrm{class}
=
\operatorname*{argmax}
\left(
p_{\mathrm{none}},
p_{\mathrm{leak}},
p_{\mathrm{constriction}}
\right).
$$

The class mapping is:

```text
0 -> NONE
1 -> LEAK
2 -> CONSTRICTION
```

---

# 4. 206-Feature Construction

## 4.1 Pressure to Hydraulic Head

For pressure measurements:

$$
H=\frac{P}{\rho g}+z.
$$

The density and elevation convention must be consistent with the training setup.

## 4.2 Steady Baseline

From the pre-transient sensor samples:

$$
H_{1,0}=\operatorname{mean}(H_{1,\mathrm{pre}})
$$

$$
H_{2,0}=\operatorname{mean}(H_{2,\mathrm{pre}}).
$$

The perturbations are:

$$
\Delta H_1(t)=H_1(t)-H_{1,0}
$$

$$
\Delta H_2(t)=H_2(t)-H_{2,0}.
$$

## 4.3 Normalized-Time Resampling

The classifier uses 100 samples per sensor.

For \(i=0,\ldots,99\):

$$
\tau_i=\frac{3.2\,i}{100}.
$$

Physical sampling time is:

$$
t_i=\frac{\tau_iL}{a_{\mathrm{est}}}.
$$

The original implementation performs linear interpolation of the baseline-subtracted sensor traces.

## 4.4 Scalar Features

Pipe area:

$$
A=\frac{\pi D^2}{4}.
$$

Nominal characteristic coefficient:

$$
B_{\mathrm{nom}}
=
\frac{a_{\mathrm{est}}}{gA}.
$$

Darcy friction factor:

$$
f=f(Q_{\mathrm{design}}).
$$

Dimensionless friction parameter:

$$
\phi
=
\frac{fLg}
{2Da_{\mathrm{est}}^2}.
$$

Initial valve flow quantity:

$$
q_{v0}=B_{\mathrm{nom}}Q_{\mathrm{design}}.
$$

First sensor position:

$$
\xi_1=\frac{x_1}{L}.
$$

The six scalar features appended after the 200 transient samples are:

```text
log(phi)
H1_0 / 100
H2_0 / 100
log(B_nom)
log(qv0)
xi1
```

---

# 5. Conditional Interpretation of Classifier Outputs

Only regression outputs associated with the predicted class should be used.

## 5.1 NONE

When:

```text
argmax(first three outputs) == 0
```

display:

```text
NONE
```

Ignore:

```text
xi_frac
kappa_leak
kappa_constriction
```

These values are not meaningful for a `NONE` prediction.

## 5.2 LEAK

When:

```text
argmax(first three outputs) == 1
```

use:

$$
\xi_{\mathrm{anomaly}}=\xi_{\mathrm{frac}}
$$

and:

$$
\kappa=\kappa_{\mathrm{leak}}.
$$

Convert normalized position to metres:

$$
x_{\mathrm{anomaly}}
=
\xi_{\mathrm{anomaly}}L.
$$

The physical leak conductance can be recovered from the repository relationship:

$$
C_dA_{\mathrm{leak}}
=
\frac{\kappa_{\mathrm{leak}}}
{B_{\mathrm{nom}}\sqrt{2g}}.
$$

Only `kappa_leak` should be used for a `LEAK` prediction.

## 5.3 CONSTRICTION

When:

```text
argmax(first three outputs) == 2
```

use:

$$
\xi_{\mathrm{anomaly}}=\xi_{\mathrm{frac}}
$$

and:

$$
\kappa=\kappa_{\mathrm{constriction}}.
$$

Convert normalized position to metres:

$$
x_{\mathrm{anomaly}}
=
\xi_{\mathrm{anomaly}}L.
$$

The corresponding constriction conductance relationship is:

$$
C_dA_c
=
\frac{\kappa_{\mathrm{constriction}}}
{B_{\mathrm{nom}}\sqrt{2g}}.
$$

Only `kappa_constriction` should be used for a `CONSTRICTION` prediction.

---

# 6. Final Display Pipeline

The two model outputs are intentionally kept separate.

### Step 1 — Generate the pressure/flow background

Create the normalized grid:

```python
xi  = linspace(0, 1, Nx)
tau = linspace(0, tau_end, Nt, endpoint=False)
```

Evaluate the forward model:

$$
[\xi,\tau]\rightarrow[h,q].
$$

Reconstruct:

$$
H=H_{ss}+H_s h
$$

and, when required:

$$
P=\rho gH.
$$

### Step 2 — Run the anomaly classifier

Construct the 206-feature vector from the two pressure sensors and invoke the classifier once.

### Step 3 — Overlay the anomaly

The final display is:

$$
\boxed{
\text{PINN pressure field}
+
\text{anomaly annotation}
}
$$

It is **not**:

$$
\boxed{
\text{PINN pressure field re-solved using the classifier prediction}
}
$$

For example, if:

```text
class = LEAK
xi_frac = 0.42
L = 100 m
```

then:

$$
x_{\mathrm{anomaly}}=0.42(100)=42\ \mathrm{m}.
$$

The display places a marker at that position and shows the leak severity.

---

# 7. Model Validation

## 7.1 Physics-Based Validation

The 2-input forward PINN was evaluated against the repository's Method-of-Characteristics (MOC) reference solution.

Reported metrics:

| Metric | Result |
|---|---:|
| H-field NRMSE | **0.586%** |
| Q-field NRMSE | **2.269%** |
| Q RMSE | **0.008608 L/s** |
| Q relative error | **0.423%** |
| Mid-pipe peak-Q error | **0.000448 L/s** |
| Sensor-head RMSE | **0.04238 m** |
| Trainable parameters | **2,274** |

The repository also reports approximately **85.3% held-out classification accuracy** for the 206-to-6 anomaly-classification model.

## 7.2 Export Validation

The float32 deployment export was checked against the original model.

Reported maximum absolute output difference:

$$
3.57\times10^{-6}.
$$

The deployed network uses a float32 interface, suitable for embedded inference.

## 7.3 Hardware Validation

> **Hardware validation completed.**

Both deployable TFLite models have been **executed and validated on the target embedded hardware**:

- **2-input forward PINN**
- **206-input, 6-output anomaly classifier**

Hardware validation confirms successful deployment and inference of the exported models on the embedded target, rather than relying only on desktop/ONNX validation.

The hardware validation status is therefore:

| Component | Hardware status |
|---|---|
| 2-input forward PINN | **Validated on hardware** |
| 206-input anomaly classifier | **Validated on hardware** |
| End-to-end inference pipeline | **Hardware validated** |

Hardware-specific latency, peak arena usage, and frame/update rate should be reported separately when exact board measurements are available.

---

# 8. Embedded Execution Order

## Event Capture

```text
1. Capture synchronized pressure samples from sensor 1 and sensor 2.
2. Retain the pre-transient samples.
3. Convert measurements to hydraulic head.
```

## Feature Construction

```text
4. Compute H1_0 and H2_0.
5. Subtract the steady baselines.
6. Estimate wave speed a_est.
7. Generate the 100-point normalized-time grid.
8. Linearly interpolate both traces.
9. Compute phi, B_nom, qv0 and xi1.
10. Assemble the 206-element float32 vector.
```

## Classification

```text
11. Run anomaly_classifier_fp32.tflite.
12. Select NONE / LEAK / CONSTRICTION using argmax.
13. Use xi_frac only for LEAK or CONSTRICTION.
14. Use kappa_leak only for LEAK.
15. Use kappa_constriction only for CONSTRICTION.
```

## Pressure Map

```text
16. Generate the normalized (xi, tau) grid.
17. Run the 2-input forward TFLite model.
18. Reconstruct H(xi, tau).
19. Convert H to pressure when required.
20. Store/render P_grid.
```

## Display

```text
21. Render P_grid as the base heatmap.
22. Overlay the anomaly marker only if class != NONE.
23. Display only the severity associated with the predicted class.
24. Optionally display class probabilities.
```

---

# 9. Recommended MCU Display Resolution

For a small embedded display, a useful starting point is:

```text
Nx = 32
Nt = 16
```

which requires:

$$
32\times16=512
$$

forward-model evaluations.

A larger display can use:

```text
Nx = 64
Nt = 24
```

requiring:

$$
64\times24=1536
$$

forward-model evaluations.

Physical axes are:

$$
x=\xi L
$$

and:

$$
t=\frac{\tau L}{a_{\mathrm{nom}}}.
$$

---

# 10. Repository Files

## Model Implementation

```text
leakpinn/pinn.py
```

Single-pipe forward PINN architecture, normalization and physical reconstruction.

```text
leakpinn/classify_net.py
```

206-to-6 anomaly classifier and deployment normalization wrapper.

```text
leakpinn/localize_data.py
```

Authoritative 206-feature definition.

```text
leakpinn/classify_data.py
```

None/leak/constriction classifier data generation.

## Physics and Data Generation

```text
leakpinn/physics.py
```

Pipe, fluid, valve, leak and constriction relationships.

```text
leakpinn/moc.py
```

Method-of-Characteristics transient solver used as the physics reference.

```text
leakpinn/synth.py
```

Synthetic sensor transient generation, noise and ADC quantization.

```text
leakpinn/domain.py
```

Randomized pipe and anomaly scenario generation.

## Training and Export

```text
scripts/02_forward_pinn.py
scripts/05_export_onnx.py
scripts/16_train_classifier.py
scripts/17_export_classifier_onnx.py
scripts/18_convert_classifier_tflite.py
```

## Deployment Documentation

```text
DEPLOYMENT.md
CONSTRICTION_DETECTION_PLAN.md
README.md
```

## Model Artifacts

```text
results/05_pinn_forward_fp32.onnx
results/05_tflite_model/<generated .tflite>
results/anomaly_classifier_fp32.tflite
```

---

# 11. Runtime Requirements

Once the models are deployed, the MCU does **not** need the training-time MOC solver or Python training stack.

Runtime requirements are:

```text
- 2-input forward TFLite model
- 206-input anomaly-classifier TFLite model
- TFLite/TFLite Micro runtime
- sensor acquisition
- embedded feature preprocessing
- pressure/head conversion
- display rendering
```

The MOC solver, synthetic-data generation and training scripts are used during development and validation rather than normal MCU inference.

---

# 12. Important Scope

The two models have different roles.

### Forward PINN

The 2-input model is a **fixed-scenario forward model**:

$$
(\xi,\tau)\rightarrow(h,q).
$$

It generates the pressure/flow background for the scenario it was trained and exported for.

### Anomaly Classifier

The 206-input classifier is an **anomaly inference model**:

$$
X_{206}
\rightarrow
[p_{\mathrm{none}},p_{\mathrm{leak}},p_{\mathrm{constriction}},
\xi,\kappa_L,\kappa_C].
$$

It determines:

```text
what happened
where it happened
which severity output is relevant
```

The classifier does not physically re-solve the pressure PDE.

---

# 13. Summary

The complete embedded system is:

```text
                    TWO PRESSURE SENSORS
                             |
                             v
                    206 FEATURE VECTOR
                             |
                             v
                +--------------------------+
                | 206 -> 6 CLASSIFIER     |
                +--------------------------+
                             |
              +--------------+--------------+
              |              |              |
              v              v              v
            NONE            LEAK       CONSTRICTION
                             |
                             | location + severity
                             v
Normalized (xi,tau) --> 2-input forward PINN
                             |
                             v
                           [h,q]
                             |
                             v
                    H(xi,tau), P(xi,tau)
                             |
                             v
                   ANOMALY-ANNOTATED MAP
                             |
                             v
                          DISPLAY
```

The deployed pipeline combines:

- **physics-informed forward modelling**
- **sensor-based anomaly classification**
- **location estimation**
- **class-specific severity estimation**
- **embedded TFLite inference**
- **hardware-validated deployment**

The final result is an embedded transient pressure/flow visualization with an anomaly marker indicating the detected event along the pipe.
