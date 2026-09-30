/*
 * pinn_pipeline.h
 *
 * Encapsulates pre-processing, synthetic transient sensor generation,
 * feature extraction for the 206-float classifier, and execution routines
 * for both the Anomaly Classifier and PINN Flow models.
 */

#ifndef PINN_PIPELINE_H_
#define PINN_PIPELINE_H_

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

// Numerical Constants
#define G_ACCEL                   (9.80665f)
#define TAU_WINDOW                (3.2f)
#define FEATURE_DIM               (206U)
#define TRACE_FEATURES            (100U)
#define DEFAULT_FS_HZ             (1000.0f)

// Feature clamping limits to prevent log(0)
#define LOG_CLAMP_PHI             (1e-12f)
#define LOG_CLAMP_B               (1e-9f)
#define LOG_CLAMP_QV0             (1e-9f)

// Memory constraints
#define MAX_TRANSIENT_SAMPLES     (500U)
#define MAX_PRE_SAMPLES           (64U)

// PINN Spatial Grid Size
#define GRID_DIM                  (32U)

// Model Scratchpad Buffer Sizes
#define CLASSIFIER_BUFFER_SIZE    (1664U)

// Pipe Physical Configuration Structure
typedef struct {
    float length;           // [m]
    float diameter;         // [m]
    float roughness;        // [m]
    float wall_thickness;   // [m]
    float young_modulus;    // [Pa]
    float poisson_ratio;    // Dimensionless
    float rho;              // Fluid density [kg/m^3]
    float mu;               // Dynamic viscosity [Pa*s]
    float bulk_modulus;     // Fluid bulk modulus [Pa]
    float q_design;         // Design flow rate [m^3/s]
    float sensor1_x;        // Upstream position [m]
    float sensor2_x;        // Downstream position [m]
    float sensor1_z;        // Upstream elevation [m]
    float sensor2_z;        // Downstream elevation [m]
    float nominal_wave_speed; // Fallback wave speed [m/s]
} PipeConfig;

// Diagnostics Output Container
typedef struct {
    float features[FEATURE_DIM];
    float raw_p1_transient[MAX_TRANSIENT_SAMPLES];
    float raw_p2_transient[MAX_TRANSIENT_SAMPLES];
    float raw_p1_pre[MAX_PRE_SAMPLES];
    float raw_p2_pre[MAX_PRE_SAMPLES];
    uint32_t num_transient;
    uint32_t num_pre;

    // Model Predictions
    float cls_outputs[6];   // Classifier predictions [xi, kappa, status, ...]

    // Reconstructed Spatial Grid Arrays (32x32)
    float h_vals[GRID_DIM * GRID_DIM];
    float q_vals[GRID_DIM * GRID_DIM];
    float H_vals[GRID_DIM * GRID_DIM];
    float P_vals[GRID_DIM * GRID_DIM];

    // Health Status Summary
    float predicted_xi;     // Leak location (0.0 to 1.0)
    float predicted_kappa;  // Leak severity coefficient
    bool  anomaly_detected; // True if leak/anomaly is flagged
} PipelineData;

// Function Prototypes
void pinn_get_default_pipe_config(PipeConfig *cfg);
void pinn_generate_synthetic_transients(PipeConfig *cfg, PipelineData *data, bool simulate_leak);
int  pinn_build_feature_vector(const PipeConfig *cfg, PipelineData *data);

// Model Inference Wrappers
void pinn_run_classifier(PipelineData *data, uint8_t *scratchpad);
void pinn_run_flow_grid(const PipeConfig *cfg, PipelineData *data, uint8_t *scratchpad);

#endif /* PINN_PIPELINE_H_ */
