/* generated vector header file - do not edit */
#ifndef VECTOR_DATA_H
#define VECTOR_DATA_H
#ifdef __cplusplus
        extern "C" {
        #endif
/* Number of interrupts allocated */
#ifndef VECTOR_DATA_IRQ_COUNT
#define VECTOR_DATA_IRQ_COUNT    (2)
#endif
/* ISR prototypes */
void drw_int_isr(void);
void glcdc_line_detect_isr(void);

/* Vector table allocations */
#define VECTOR_NUMBER_DRW_INT ((IRQn_Type) 0) /* DRW INT (DRW interrupt) */
#define DRW_INT_IRQn          ((IRQn_Type) 0) /* DRW INT (DRW interrupt) */
#define VECTOR_NUMBER_GLCDC_LINE_DETECT ((IRQn_Type) 1) /* GLCDC LINE DETECT (Specified line) */
#define GLCDC_LINE_DETECT_IRQn          ((IRQn_Type) 1) /* GLCDC LINE DETECT (Specified line) */
/* The number of entries required for the ICU vector table. */
#define BSP_ICU_VECTOR_NUM_ENTRIES (2)

#ifdef __cplusplus
        }
        #endif
#endif /* VECTOR_DATA_H */
