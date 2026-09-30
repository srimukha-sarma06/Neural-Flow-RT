################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
../mtk3_bsp2/mtkernel/kernel/sysdepend/cpu/core/armv7m/dispatch.S 

C_SRCS += \
../mtk3_bsp2/mtkernel/kernel/sysdepend/cpu/core/armv7m/cpu_cntl.c \
../mtk3_bsp2/mtkernel/kernel/sysdepend/cpu/core/armv7m/exc_hdr.c \
../mtk3_bsp2/mtkernel/kernel/sysdepend/cpu/core/armv7m/interrupt.c \
../mtk3_bsp2/mtkernel/kernel/sysdepend/cpu/core/armv7m/reset_hdl.c 

C_DEPS += \
./mtk3_bsp2/mtkernel/kernel/sysdepend/cpu/core/armv7m/cpu_cntl.d \
./mtk3_bsp2/mtkernel/kernel/sysdepend/cpu/core/armv7m/exc_hdr.d \
./mtk3_bsp2/mtkernel/kernel/sysdepend/cpu/core/armv7m/interrupt.d \
./mtk3_bsp2/mtkernel/kernel/sysdepend/cpu/core/armv7m/reset_hdl.d 

OBJS += \
./mtk3_bsp2/mtkernel/kernel/sysdepend/cpu/core/armv7m/cpu_cntl.o \
./mtk3_bsp2/mtkernel/kernel/sysdepend/cpu/core/armv7m/dispatch.o \
./mtk3_bsp2/mtkernel/kernel/sysdepend/cpu/core/armv7m/exc_hdr.o \
./mtk3_bsp2/mtkernel/kernel/sysdepend/cpu/core/armv7m/interrupt.o \
./mtk3_bsp2/mtkernel/kernel/sysdepend/cpu/core/armv7m/reset_hdl.o 

SREC += \
neural_flow.srec 

S_UPPER_DEPS += \
./mtk3_bsp2/mtkernel/kernel/sysdepend/cpu/core/armv7m/dispatch.d 

MAP += \
neural_flow.map 


# Each subdirectory must supply rules for building sources it contributes
mtk3_bsp2/mtkernel/kernel/sysdepend/cpu/core/armv7m/%.o: ../mtk3_bsp2/mtkernel/kernel/sysdepend/cpu/core/armv7m/%.c
	$(file > $@.in,-mthumb -mfloat-abi=hard -mcpu=cortex-m85+nopacbti -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-strict-aliasing -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g -D_RENESAS_RA_ -D_RAFSP_EK_RA8P1_ -D_RA_CORE=CPU0 -D_RA_ORDINAL=1 -I"C:/Users/skull/e2_studio/workspace/neural_flow/mtk3_bsp2" -I"C:/Users/skull/e2_studio/workspace/neural_flow/mtk3_bsp2/mtkernel/kernel/knlinc" -I"C:/Users/skull/e2_studio/workspace/neural_flow/mtk3_bsp2/include" -I"C:/Users/skull/e2_studio/workspace/neural_flow/mtk3_bsp2/config" -I"C:/Users/skull/e2_studio/workspace/neural_flow/ra_cfg/fsp_cfg/bsp" -I"." -I"C:/Users/skull/e2_studio/workspace/neural_flow/ra_gen" -I"C:/Users/skull/e2_studio/workspace/neural_flow/ra_cfg/fsp_cfg" -I"C:/Users/skull/e2_studio/workspace/neural_flow/ra/fsp/inc" -I"C:/Users/skull/e2_studio/workspace/neural_flow/ra/tes/dave2d/inc" -I"C:/Users/skull/e2_studio/workspace/neural_flow/src" -I"C:/Users/skull/e2_studio/workspace/neural_flow/ra/fsp/inc/api" -I"C:/Users/skull/e2_studio/workspace/neural_flow/ra/fsp/inc/instances" -I"C:/Users/skull/e2_studio/workspace/neural_flow/ra/fsp/src/r_drw" -I"C:/Users/skull/e2_studio/workspace/neural_flow/ra/arm/CMSIS_6/CMSIS/Core/Include" -std=c99 -Wno-stringop-overflow -Wno-format-truncation -flax-vector-conversions --param=min-pagesize=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"
mtk3_bsp2/mtkernel/kernel/sysdepend/cpu/core/armv7m/%.o: ../mtk3_bsp2/mtkernel/kernel/sysdepend/cpu/core/armv7m/%.S
	$(file > $@.in,-mthumb -mfloat-abi=hard -mcpu=cortex-m85+nopacbti -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-strict-aliasing -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g -x assembler-with-cpp -D_RENESAS_RA_ -D_RAFSP_EK_RA8P1_ -D_RA_CORE=CPU0 -D_RA_ORDINAL=1 -I"C:/Users/skull/e2_studio/workspace/neural_flow/ra_cfg/fsp_cfg/bsp" -I"C:/Users/skull/e2_studio/workspace/neural_flow/mtk3_bsp2/mtkernel/kernel/knlinc" -I"C:/Users/skull/e2_studio/workspace/neural_flow/mtk3_bsp2/include" -I"C:/Users/skull/e2_studio/workspace/neural_flow/mtk3_bsp2/config" -I"C:/Users/skull/e2_studio/workspace/neural_flow/mtk3_bsp2" -I"." -I"C:/Users/skull/e2_studio/workspace/neural_flow/ra_gen" -I"C:/Users/skull/e2_studio/workspace/neural_flow/ra_cfg/fsp_cfg" -I"C:/Users/skull/e2_studio/workspace/neural_flow/ra/fsp/inc" -I"C:/Users/skull/e2_studio/workspace/neural_flow/ra/tes/dave2d/inc" -I"C:/Users/skull/e2_studio/workspace/neural_flow/src" -I"C:/Users/skull/e2_studio/workspace/neural_flow/ra/fsp/inc/api" -I"C:/Users/skull/e2_studio/workspace/neural_flow/ra/fsp/inc/instances" -I"C:/Users/skull/e2_studio/workspace/neural_flow/ra/fsp/src/r_drw" -I"C:/Users/skull/e2_studio/workspace/neural_flow/ra/arm/CMSIS_6/CMSIS/Core/Include" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"

