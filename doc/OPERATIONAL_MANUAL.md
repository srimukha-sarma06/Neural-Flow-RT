# NeuralFlow-RT: Physics-Informed Digital Twin — Operation & Evaluation Manual

## 1. System Specifications & Environment

To compile, flash, and evaluate the **NeuralFlow-RT** real-time fluid digital twin on the target evaluation board, ensure your host environment matches the following toolchain and hardware configuration:

| Component | Specification |
| :--- | :--- |
| **Evaluation Board** | Renesas EK-RA8P1 Evaluation Kit |
| **Target Microcontroller** | Renesas RA8P1 (`R7KA8P1KFLCAC` — Cortex-M85 with ARM Helium MVE) |
| **Integrated Development Environment** | Renesas e² studio (Embedded Firmware) / VS Code (Python AI & OpenFOAM) |
| **Flexible Software Package (FSP)** | Version `6.5.0` |
| **Toolchain** | GCC ARM Embedded `13.2.1.arm-13-7` |
| **Arm CMSIS Package** | Arm CMSIS Version 6 - Core (M) `v6.1.0+fsp.6.5.0` & CMSIS-NN |
| **Target OS** | μT-Kernel 3.0 RTOS (BSP2 / Flat Multi-Core CPU0) |
| **Display & Graphics Engine** | 1024×600 RGB TFT LCD via GLCDC & D2D Hardware Acceleration |
| **Development Team** | Srriram (HW/FW), Srimukha (AI/Quantization), Mayuur (AI/Quantization) |

---

## 2. Directory Structure

```text
.
├── demo_project/               # Ready-to-import e² studio project
│   ├── src/
│   │   ├── usermain.c          # RTOS task orchestration & display thread
│   │   ├── pinn_pipeline.c     # Preprocessor, transient generator & PINN solvers
│   │   ├── lcd_lib.c           # D2D/GLCDC graphics driver & jet colormap renderer
│   │   └── flow_model/         # Quantized PINN C model headers (compute_sub_0000.h)
│   └── inc/
│       ├── pinn_pipeline.h     # Pipeline data structures & function signatures
│       └── lcd_lib.h           # Display parameters & HeatmapConfig definitions
├── python_model/               # Offline Navier-Stokes PINN training & OpenFOAM scripts
├── doc/
│   └── OPERATION_MANUAL.md     # System evaluation and operation manual
├── assets/
│   ├── flow_perturbation.png   # Output heatmap for flow (q) and head (h) perturbations
│   ├── reconstructed_pressure.png # Output heatmap for physical pressure field [bar]
│   └── benchmark.png          # Execution metrics (Inference time vs GPU/Display rendering)
└── LICENSE                     # MIT Open Source License
```

---

## 3. Part 1: Quick Evaluation via demo Project

This section provides step-by-step instructions for evaluating the pre-compiled two-stage PINN edge digital twin on the Renesas EK-RA8P1 board.

### Step 1: Import Project into e² studio
1. Launch **Renesas e² studio**.
2. Select your workspace location.
3. Select **File → Import...** from the main menu.
4. Expand **General**, choose **Existing Projects into Workspace**, and click **Next**.
5. Choose **Select root directory**, click **Browse...**, and navigate to the `demo_project` folder inside this repository.
6. Ensure `demo_project` is checked in the *Projects* panel and click **Finish**.

### Step 2: Build the Project (Debug / Release Mode)
1. In the **Project Explorer**, right-click `demo_project`.
2. Click **Build Project** (or press `Ctrl + B`).
3. Verify in the e² studio Console tab that compilation completes with **0 Errors**.

> **Note:** If you encounter semantic indexer warnings in e² studio (such as unresolved symbols in `hal_data.h`), you can safely ignore them as long as the build completes with 0 compilation errors.

### Step 3: Flash and Launch Debugger
1. Connect the **Renesas EK-RA8P1** board to your host PC using a micro-USB cable on the J-Link Debug Port (`J10`).
2. Power on the TFT LCD module connected to the GLCDC expansion header.
3. In e² studio, click **Run → Debug Configurations...**.
4. Under **Renesas GDB Hardware Debugging**, select `demo_project`.
5. Verify target settings: J-Link ARM interface connected to target device `R7KA8P1KFLCAC`.
6. Click **Debug** to flash the microcontroller and initialize the GDB debug perspective.

### Step 4: Run Real-Time Display & Operational Verification Criteria
Click **Resume (F8)** in e² studio to start execution under μT-Kernel 3.0.

- **Deterministic Multi-Tasking Execution:** Upon boot, `usermain.c` creates and launches two RTOS tasks:
  - `task_pinn_inference` (Priority 10): Synthesizes transient data, calculates the 206-float feature vector, executes Stage-1 Anomaly Classifier, and solves the Stage-2 32×32 Spatial PINN Flow Grid.
  - `task_lcd_render` (Priority 11): Handles hardware-accelerated bilinear filtering, Jet-colormap thermal conversion, and screen rendering via the D2D engine.

- **Real-Time Display Output Modes:** The display automatically cycles every 3 seconds across 4 spatial field views:
  1. **PINN: HEAD PERTURBATION ($h$):** Dimensionless fluid head fluctuation grid.
  2. **PINN: FLOW PERTURBATION ($q$):** Dimensionless flow velocity perturbation grid.
  3. **PHYSICAL HEAD $H(\xi, \tau)$ [m]:** Reconstructed physical head along pipe normalized space $\xi$ and window time $\tau$.
  4. **RECONSTRUCTED PRESSURE [bar]:** Complete 2D pressure field map.

![Flow Perturbation Heatmap Output](assets/flow_perturbation.png)

![Reconstructed Pressure Map Output](assets/reconstructed_pressure.png)

- **Anomaly Detection Verification:**
  - **Healthy Pipeline State:** The header displays `"SYSTEM HEALTHY: NO LEAK DETECTED"` in green text.
  - **Simulated Leak Anomaly State:** The header turns red with the banner `"ANOMALY DETECTED | XI:<val> | KAPPA:<val>"`, indicating the predicted leak location ($\xi$) and magnitude ($\kappa$).

- **Execution Metrics & Performance Display:**
  - `INF:<ms>` displays total two-stage PINN execution latency accelerated by Cortex-M85 Helium SIMD instructions.
  - `GPU:<ms>` displays hardware frame rendering time using D2D bit-blit and texture mapping.

![Performance Benchmark Output](assets/benchmark.png)

---

## 4. Part 2: Integrating Firmware with Physical Hardware Sensors

The current demo pipeline uses `pinn_generate_synthetic_transients()` to simulate transient pressure waves. To transition from synthetic data to live physical ADC pressure sensors (placed at upstream location $X_1$ and downstream location $X_2$):

### Step 1: Replace Synthetic Data Polling
In `usermain.c`, replace the call to `pinn_generate_synthetic_transients()` inside `task_pinn_inference` with your DMA/ADC buffer driver routine:

```c
// Replace this synthetic call in task_pinn_inference:
// pinn_generate_synthetic_transients(&g_pipe_cfg, &g_pipeline_data, simulate_anomaly_toggle);

// With live DMA buffer polling:
adc_read_dma_buffers(g_pipeline_data.raw_p1_transient, 
                     g_pipeline_data.raw_p2_transient, 
                     MAX_TRANSIENT_SAMPLES);
```

### Step 2: Middleware API Function Reference

| Function Name | Description | Inputs / Outputs |
| :--- | :--- | :--- |
| **`pinn_get_default_pipe_config`** | Initializes pipe geometry ($L, D, e$), fluid density ($\rho$), viscosity ($\mu$), and wave speed ($a$). | `PipeConfig *cfg` |
| **`pinn_build_feature_vector`** | Converts raw transient pressure arrays into normalized 206-float feature vector. | `PipeConfig *cfg, PipelineData *data` |
| **`pinn_run_classifier`** | Stage-1 inference: Predicts leak presence, location ($\xi$), and severity ($\kappa$). | `PipelineData *data, uint8_t *scratchpad` |
| **`pinn_run_flow_grid`** | Stage-2 inference: Solves 32×32 Navier-Stokes grid ($h, q, H, P$) across spatial domain. | `PipeConfig *cfg, PipelineData *data, uint8_t *scratch` |
| **`lcd_draw_heatmap`** | Hardware-accelerated 2D Jet colormap rendering via D2D engine. | `HeatmapConfig *cfg` |
| **`lcd_draw_string`** | Renders dynamic text banners using full 8×8 ASCII font atlas. | `char *text, x, y, color, scale` |

---

## 5. Technical Notes & Architectural Highlights

- **Physics-Informed Architecture:** Unlike purely data-driven models, NeuralFlow-RT embeds Navier-Stokes mass and momentum conservation equations directly into the network loss function during training, preventing unphysical fluid predictions.
- **Hardware SIMD Acceleration:** Uses ARM Helium vector exten
