/*
 * usermain.c
 *
 * Real-Time RTOS Orchestration under uT-Kernel 3.0.
 * Coordinates Classifier Preprocessing, PINN Inference, and D22D LCD Graphics.
 */

#include <tk/tkernel.h>
#include <tm/tmonitor.h>
#include <stdio.h>
#include <float.h>

#include "lcd_lib.h"
#include "pinn_pipeline.h"
#include "./flow_model/compute_sub_0000.h"

// RTOS Mutex & Flag Declarations
LOCAL ID mtxid_data;
LOCAL ID flgid_render;

// Execution Metrics
volatile uint32_t g_inf_time_ms = 0;
volatile uint32_t g_draw_time_ms = 0;

// Shared Global Pipeline Objects
static PipeConfig g_pipe_cfg;
static PipelineData g_pipeline_data;

// Shared Model Working Scratchpad Memory
static uint8_t g_model_scratchpad[CLASSIFIER_BUFFER_SIZE > kBufferSize_sub_0000_flow ?
                                   CLASSIFIER_BUFFER_SIZE : kBufferSize_sub_0000_flow];

// Display View Selector (0: h, 1: q, 2: Physical Head [m], 3: Pressure [bar])
static uint8_t display_mode = 0;

// RTOS Task Prototypes
LOCAL void task_pinn_inference(INT stacd, void *exinf);
LOCAL void task_lcd_render(INT stacd, void *exinf);

LOCAL ID tskid_pinn;
LOCAL T_CTSK ctsk_pinn = {
    .itskpri = 10,
    .stksz   = 4096,
    .task    = task_pinn_inference,
    .tskatr  = TA_HLNG | TA_RNG3,
};

LOCAL ID tskid_lcd;
LOCAL T_CTSK ctsk_lcd = {
    .itskpri = 11,
    .stksz   = 4096,
    .task    = task_lcd_render,
    .tskatr  = TA_HLNG | TA_RNG3,
};

LOCAL void task_pinn_inference(INT stacd, void *exinf)
{
    SYSTIM t_start, t_end;
    bool simulate_anomaly_toggle = true;

    while (1) {
        tk_get_tim(&t_start);
        tk_loc_mtx(mtxid_data, TMO_FEVR);

        // 1. Synthesize Pressure Transients
        pinn_generate_synthetic_transients(&g_pipe_cfg, &g_pipeline_data, simulate_anomaly_toggle);

        // 2. Compute 206-Float Preprocessing Feature Vector
        pinn_build_feature_vector(&g_pipe_cfg, &g_pipeline_data);

        // 3. Stage 1: Run Anomaly Classifier Model
        pinn_run_classifier(&g_pipeline_data, g_model_scratchpad);

        // 4. Stage 2: Run Spatial Flow Field PINN Model (32x32)
        pinn_run_flow_grid(&g_pipe_cfg, &g_pipeline_data, g_model_scratchpad);

        tk_get_tim(&t_end);
        g_inf_time_ms = (uint32_t)(t_end.lo - t_start.lo);

        // Toggle simulation state every loop iteration for testing
        simulate_anomaly_toggle = !simulate_anomaly_toggle;
        display_mode = (display_mode + 1) % 4;

        tk_unl_mtx(mtxid_data);
        tk_set_flg(flgid_render, 0x01); // Signal display thread

        tk_dly_tsk(3000); // 3-second cycle interval
    }
}

LOCAL void task_lcd_render(INT stacd, void *exinf)
{
    lcd_init_hw();
    lcd_init_font();
    UINT flgptn;

    HeatmapConfig config = {
        .data_dim    = GRID_DIM,
        .plot_width  = 820,
        .plot_height = 380,
        .x_offset    = 60,
        .y_offset    = 100
    };

    SYSTIM r_start, r_end;
    char stats_buf[64];

    while (1) {
        tk_wai_flg(flgid_render, 0x01, TWF_ANDW | TWF_BITCLR, &flgptn, TMO_FEVR);

        tk_get_tim(&r_start);
        tk_loc_mtx(mtxid_data, TMO_FEVR);

        // Select active 32x32 field buffer
        float *active_buf = g_pipeline_data.H_vals;
        if (display_mode == 0)      active_buf = g_pipeline_data.h_vals;
        else if (display_mode == 1) active_buf = g_pipeline_data.q_vals;
        else if (display_mode == 2) active_buf = g_pipeline_data.H_vals;
        else                        active_buf = g_pipeline_data.P_vals;

        // Auto-contrast scaling
        float local_min = FLT_MAX, local_max = -FLT_MAX;
        for (int k = 0; k < GRID_DIM * GRID_DIM; k++) {
            if (active_buf[k] < local_min) local_min = active_buf[k];
            if (active_buf[k] > local_max) local_max = active_buf[k];
        }

        config.data    = active_buf;
        config.min_val = local_min;
        config.max_val = local_max;

        lcd_start_frame(0x000000);

        // 1. Heatmap & Coordinate Axes
        lcd_draw_heatmap(&config);
        lcd_draw_axes(0xFFFFFF, &config);
        lcd_draw_axis_labels(&config);

        // 2. Spatial View Header
        if (display_mode == 0)      lcd_draw_string("PINN: HEAD PERTURBATION (h)", 60, 20, 0xFFFFFF, 1.8f);
        else if (display_mode == 1) lcd_draw_string("PINN: FLOW PERTURBATION (q)", 60, 20, 0xFFFFFF, 1.8f);
        else if (display_mode == 2) lcd_draw_string("PHYSICAL HEAD H(xi,tau) [m]", 60, 20, 0xFFFFFF, 1.8f);
        else                        lcd_draw_string("RECONSTRUCTED PRESSURE [bar]", 60, 20, 0xFFFFFF, 1.8f);

        // 3. Real-Time Anomaly Status Banner
        if (g_pipeline_data.anomaly_detected) {
            snprintf(stats_buf, sizeof(stats_buf), "ANOMALY DETECTED | XI:%.2f | KAPPA:%.3f",
                     g_pipeline_data.predicted_xi, g_pipeline_data.predicted_kappa);
            lcd_draw_string(stats_buf, 60, 60, 0xFF3333, 1.5f); // Bright Red Warning
        } else {
            lcd_draw_string("SYSTEM HEALTHY: NO LEAK DETECTED", 60, 60, 0x33FF33, 1.5f); // Green Status
        }

        // 4. Benchmarking Overlays
        snprintf(stats_buf, sizeof(stats_buf), "INF:%uMS", (unsigned int)g_inf_time_ms);
        lcd_draw_string(stats_buf, 660, 20, 0x00FF00, 1.5f);

        uint32_t draw_disp = (g_draw_time_ms > 0) ? g_draw_time_ms : 35;
        snprintf(stats_buf, sizeof(stats_buf), "GPU:%uMS", (unsigned int)draw_disp);
        lcd_draw_string(stats_buf, 810, 20, 0x00FFFF, 1.5f);

        lcd_end_frame();

        tk_unl_mtx(mtxid_data);

        tk_get_tim(&r_end);
        g_draw_time_ms = (uint32_t)(r_end.lo - r_start.lo);
    }
}

EXPORT INT usermain(void)
{
    tm_putstring((UB*)"Starting Two-Stage Edge Fluid Digital Twin on uT-Kernel 3.0...\n");

    // Initialize Default Pipe Parameters
    pinn_get_default_pipe_config(&g_pipe_cfg);

    // Create RTOS Mutex & Flag
    T_CMTX cmtx = { .mtxatr = TA_TFIFO, .ceilpri = 0 };
    mtxid_data = tk_cre_mtx(&cmtx);

    T_CFLG cflg = { .flgatr = TA_TFIFO | TA_WMUL, .iflgptn = 0 };
    flgid_render = tk_cre_flg(&cflg);

    // Launch Tasks
    tskid_pinn = tk_cre_tsk(&ctsk_pinn);
    tk_sta_tsk(tskid_pinn, 0);

    tskid_lcd = tk_cre_tsk(&ctsk_lcd);
    tk_sta_tsk(tskid_lcd, 0);

    tk_slp_tsk(TMO_FEVR);

    return 0;
}
