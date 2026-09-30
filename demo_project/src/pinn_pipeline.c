/*
 * pinn_pipeline.c
 *
 * Math preprocessor, synthetic data generator, feature vector extractor,
 * and inference wrappers for the double-model architecture.
 */

#include "pinn_pipeline.h"
#include "./flow_model/compute_sub_0000.h"
#include <string.h>

// External prototype declaration for classifier function
extern void compute_sub_0000_classifier(
    uint8_t* main_storage,
    const float sensor_features[206],
    float cls_xi_kappa_70032[6]
);

void pinn_get_default_pipe_config(PipeConfig *cfg) {
    if (!cfg) return;
    cfg->length              = 500.0f;     // 500 meters
    cfg->diameter            = 0.20f;      // 20 cm
    cfg->roughness           = 0.000045f;  // Commercial steel
    cfg->wall_thickness      = 0.006f;     // 6 mm
    cfg->young_modulus       = 2.1e11f;    // Steel 210 GPa
    cfg->poisson_ratio       = 0.30f;
    cfg->rho                 = 998.2f;     // Water density
    cfg->mu                  = 1.002e-3f;  // Water viscosity
    cfg->bulk_modulus        = 2.19e9f;    // Water bulk modulus
    cfg->q_design            = 0.05f;      // 50 L/s
    cfg->sensor1_x           = 50.0f;      // 10% along pipe
    cfg->sensor2_x           = 450.0f;     // 90% along pipe
    cfg->sensor1_z           = 0.0f;
    cfg->sensor2_z           = 0.0f;
    cfg->nominal_wave_speed  = 1200.0f;    // 1200 m/s
}

void pinn_generate_synthetic_transients(PipeConfig *cfg, PipelineData *data, bool simulate_leak) {
    if (!cfg || !data) return;

    data->num_pre = 32;
    data->num_transient = 200;

    // Baseline steady-state pressures (in Pascals)
    // 50m head ~ 490,000 Pa; 45m head ~ 441,000 Pa
    float base_p1 = 490500.0f;
    float base_p2 = 441450.0f;

    // 1. Populate Steady-State Pre-Trigger Arrays
    for (uint32_t i = 0; i < data->num_pre; i++) {
        data->raw_p1_pre[i] = base_p1 + (float)((i % 3) - 1) * 10.0f;
        data->raw_p2_pre[i] = base_p2 + (float)((i % 3) - 1) * 10.0f;
    }

    // 2. Populate Transient Arrays with Synthesized Pressure Wave
    float leak_drop_p1 = simulate_leak ? 35000.0f : 0.0f;
    float leak_drop_p2 = simulate_leak ? 42000.0f : 0.0f;

    for (uint32_t i = 0; i < data->num_transient; i++) {
        float t = (float)i / DEFAULT_FS_HZ;

        // Transient wave arrival delay
        float wave_delay_1 = 0.02f;
        float wave_delay_2 = 0.05f;

        float drop1 = (t > wave_delay_1) ? leak_drop_p1 * (1.0f - expf(-50.0f * (t - wave_delay_1))) : 0.0f;
        float drop2 = (t > wave_delay_2) ? leak_drop_p2 * (1.0f - expf(-50.0f * (t - wave_delay_2))) : 0.0f;

        // Add periodic reflection oscillations
        float oscillation = simulate_leak ? (12000.0f * sinf(2.0f * M_PI * 15.0f * t) * expf(-5.0f * t)) : 0.0f;

        data->raw_p1_transient[i] = base_p1 - drop1 + oscillation;
        data->raw_p2_transient[i] = base_p2 - drop2 + (0.8f * oscillation);
    }
}

static float calculate_wave_speed_korteweg(const PipeConfig *cfg) {
    float k_over_rho = cfg->bulk_modulus / cfg->rho;
    float elastic_term = (1.0f - cfg->poisson_ratio * cfg->poisson_ratio) *
                         (cfg->bulk_modulus * cfg->diameter) /
                         (cfg->young_modulus * cfg->wall_thickness);
    float denom = 1.0f + elastic_term;
    return sqrtf(k_over_rho / denom);
}

int pinn_build_feature_vector(const PipeConfig *cfg, PipelineData *data) {
    if (!cfg || !data || data->num_pre == 0 || data->num_transient < 2) return -1;

    // Step A: Calculate Steady-State Baselines (P -> Head in meters)
    float sum_p1 = 0.0f, sum_p2 = 0.0f;
    for (uint32_t i = 0; i < data->num_pre; i++) {
        sum_p1 += data->raw_p1_pre[i];
        sum_p2 += data->raw_p2_pre[i];
    }
    float avg_p1_0 = sum_p1 / (float)data->num_pre;
    float avg_p2_0 = sum_p2 / (float)data->num_pre;

    float H1_0 = (avg_p1_0 / (cfg->rho * G_ACCEL)) + cfg->sensor1_z;
    float H2_0 = (avg_p2_0 / (cfg->rho * G_ACCEL)) + cfg->sensor2_z;

    // Step B: Extract Perturbation Arrays Delta H(t)
    static float delta_H1[MAX_TRANSIENT_SAMPLES];
    static float delta_H2[MAX_TRANSIENT_SAMPLES];

    for (uint32_t i = 0; i < data->num_transient; i++) {
        float H1_t = (data->raw_p1_transient[i] / (cfg->rho * G_ACCEL)) + cfg->sensor1_z;
        float H2_t = (data->raw_p2_transient[i] / (cfg->rho * G_ACCEL)) + cfg->sensor2_z;
        delta_H1[i] = H1_t - H1_0;
        delta_H2[i] = H2_t - H2_0;
    }

    // Step C: Estimate Wave Speed
    float a = calculate_wave_speed_korteweg(cfg);
    if (a <= 0.0f || isnan(a)) {
        a = cfg->nominal_wave_speed;
    }

    // Step D: Fluid & Dimensionless Parameters
    float area = (M_PI * cfg->diameter * cfg->diameter) / 4.0f;
    float velocity = cfg->q_design / area;
    float B_nom = a / (G_ACCEL * area);

    // Friction factor calculation
    float Re = (cfg->rho * velocity * cfg->diameter) / cfg->mu;
    float f;
    if (Re < 2000.0f) {
        f = (Re > 0.0f) ? (64.0f / Re) : 0.02f;
    } else {
        // Swamee-Jain Equation
        float term1 = cfg->roughness / (3.7f * cfg->diameter);
        float term2 = 5.74f / powf(Re, 0.9f);
        f = 0.25f / powf(log10f(term1 + term2), 2.0f);
    }

    float phi = (f * cfg->length * G_ACCEL) / (2.0f * cfg->diameter * a * a);
    float q_v0 = B_nom * cfg->q_design;

    // Step E: Resample 100 points over t_window = 3.2 * L / a
    float t_window = TAU_WINDOW * (cfg->length / a);
    float total_transient_time = (float)(data->num_transient - 1) / DEFAULT_FS_HZ;

    for (uint32_t i = 0; i < TRACE_FEATURES; i++) {
        float t_query = ((float)i / (float)(TRACE_FEATURES - 1)) * t_window;

        // Map t_query into discrete indices for linear interpolation
        float sample_idx = (t_query / total_transient_time) * (float)(data->num_transient - 1);
        int idx_low = (int)sample_idx;
        int idx_high = idx_low + 1;

        if (idx_low >= (int)data->num_transient - 1) {
            idx_low = data->num_transient - 1;
            idx_high = idx_low;
        }

        float alpha = sample_idx - (float)idx_low;

        float h1_interp = delta_H1[idx_low] + alpha * (delta_H1[idx_high] - delta_H1[idx_low]);
        float h2_interp = delta_H2[idx_low] + alpha * (delta_H2[idx_high] - delta_H2[idx_low]);

        data->features[i]                  = h1_interp;
        data->features[100 + i]            = h2_interp;
    }

    // Step F: Pack Clamped Scalar Metadata
    float phi_clamped  = (phi > LOG_CLAMP_PHI) ? phi : LOG_CLAMP_PHI;
    float B_clamped    = (B_nom > LOG_CLAMP_B) ? B_nom : LOG_CLAMP_B;
    float qv0_clamped  = (q_v0 > LOG_CLAMP_QV0) ? q_v0 : LOG_CLAMP_QV0;

    data->features[200] = logf(phi_clamped);
    data->features[201] = H1_0 / 100.0f;
    data->features[202] = H2_0 / 100.0f;
    data->features[203] = logf(B_clamped);
    data->features[204] = logf(qv0_clamped);
    data->features[205] = cfg->sensor1_x / cfg->length;

    return 0;
}

void pinn_run_classifier(PipelineData *data, uint8_t *scratchpad) {
    if (!data || !scratchpad) return;

    compute_sub_0000_classifier(scratchpad, data->features, data->cls_outputs);

    // Interpret Model Outputs
    data->predicted_xi    = data->cls_outputs[0];
    data->predicted_kappa = data->cls_outputs[1];

    // Flag anomaly if severity score exceeds threshold
    data->anomaly_detected = (data->cls_outputs[2] > 0.5f || fabsf(data->predicted_kappa) > 0.05f);
}

void pinn_run_flow_grid(const PipeConfig *cfg, PipelineData *data, uint8_t *scratchpad) {
    if (!cfg || !data || !scratchpad) return;

    float model_input[2];  // [xi, tau]
    float model_output[2]; // [h, q]

    float H1_0 = 50.0f;
    float H2_0 = 45.0f;
    float xi1  = cfg->sensor1_x / cfg->length;
    float Hs   = 10.0f; // Head scaling factor

    for (int r = 0; r < GRID_DIM; r++) {
        float tau = ((float)r / (float)(GRID_DIM - 1)) * TAU_WINDOW;

        for (int c = 0; c < GRID_DIM; c++) {
            float xi = (float)c / (float)(GRID_DIM - 1);
            int idx = r * GRID_DIM + c;

            model_input[0] = xi;
            model_input[1] = tau;

            compute_sub_0000_flow(scratchpad, model_input, model_output);

            float h_pert = model_output[0];
            float q_pert = model_output[1];

            data->h_vals[idx] = h_pert;
            data->q_vals[idx] = q_pert;

            float Hss = H2_0 + ((H1_0 - H2_0) / (1.0f - xi1)) * (1.0f - xi);
            float H_phys = Hss + (Hs * h_pert);
            if (data->anomaly_detected) {
                float dist = xi - data->predicted_xi;
                float leak_impact = fabsf(data->predicted_kappa) * expf(-40.0f * dist * dist);
                H_phys -= (leak_impact * 20.0f);
            }
            data->H_vals[idx] = H_phys;

            float P_bar = (cfg->rho * G_ACCEL * H_phys) / 100000.0f;
            data->P_vals[idx] = P_bar;
        }
    }
}
