/*
 * lcd_lib.h
 */

#ifndef LCD_LIB_H_
#define LCD_LIB_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "hal_data.h"

#define SCREEN_WIDTH    (1024U)
#define SCREEN_HEIGHT   (600U)
#define SHIFT_VALUE     (4U)
#define TEX_DIM         (128U)

// Struct to generalize heatmap parameters
typedef struct {
    float* data;
    uint16_t data_dim;
    uint16_t plot_width;
    uint16_t plot_height;
    uint16_t x_offset;
    uint16_t y_offset;
    float min_val;
    float max_val;
} HeatmapConfig;

void lcd_init_hw(void);
void lcd_init_font(void);
void lcd_start_frame(uint32_t clear_color);
void lcd_draw_axes(uint32_t color, HeatmapConfig* cfg);
void lcd_draw_axis_labels(HeatmapConfig* cfg);
void lcd_draw_heatmap(HeatmapConfig* cfg);
void lcd_draw_string(const char* text, uint16_t x, uint16_t y, uint32_t color, float scale);
void lcd_end_frame(void);

#endif /* LCD_LIB_H_ */
