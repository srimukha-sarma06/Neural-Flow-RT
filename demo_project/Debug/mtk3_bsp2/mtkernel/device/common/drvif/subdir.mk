################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../mtk3_bsp2/mtkernel/device/common/drvif/msdrvif.c 

C_DEPS += \
./mtk3_bsp2/mtkernel/device/common/drvif/msdrvif.d 

OBJS += \
./mtk3_bsp2/mtkernel/device/common/drvif/msdrvif.o 

SREC += \
neural_flow.srec 

MAP += \
neural_flow.map 


# Each subdirectory must supply rules for building sources it contributes
mtk3_bsp2/mtkernel/device/common/drvif/%.o: ../mtk3_bsp2/mtkernel/device/common/drvif/%.c
	$(file > $@.in,-mthumb -mfloat-abi=hard -mcpu=cortex-m85+nopacbti -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-strict-aliasing -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g -D_RENESAS_RA_ -D_RAFSP_EK_RA8P1_ -D_RA_CORE=CPU0 -D_RA_ORDINAL=1 -I"C:/Users/skull/e2_studio/workspace/neural_flow/ra_cfg/fsp_cfg/bsp" -I"C:/Users/skull/e2_studio/workspace/neural_flow/mtk3_bsp2" -I"C:/Users/skull/e2_studio/workspace/neural_flow/mtk3_bsp2/config" -I"C:/Users/skull/e2_studio/workspace/neural_flow/mtk3_bsp2/include" -I"C:/Users/skull/e2_studio/workspace/neural_flow/mtk3_bsp2/mtkernel/kernel/knlinc" -I"." -I"C:/Users/skull/e2_studio/workspace/neural_flow/ra_gen" -I"C:/Users/skull/e2_studio/workspace/neural_flow/ra_cfg/fsp_cfg" -I"C:/Users/skull/e2_studio/workspace/neural_flow/ra/fsp/inc" -I"C:/Users/skull/e2_studio/workspace/neural_flow/ra/tes/dave2d/inc" -I"C:/Users/skull/e2_studio/workspace/neural_flow/src" -I"C:/Users/skull/e2_studio/workspace/neural_flow/ra/fsp/inc/api" -I"C:/Users/skull/e2_studio/workspace/neural_flow/ra/fsp/inc/instances" -I"C:/Users/skull/e2_studio/workspace/neural_flow/ra/fsp/src/r_drw" -I"C:/Users/skull/e2_studio/workspace/neural_flow/ra/arm/CMSIS_6/CMSIS/Core/Include" -I"C:/Users/skull/e2_studio/workspace/neural_flow/ra/arm/CMSIS-NN/Include" -I"C:/Users/skull/e2_studio/workspace/neural_flow/ra/arm/CMSIS-NN" -I"C:/Users/skull/e2_studio/workspace/neural_flow/ra/arm/CMSIS-View/EventRecorder/Include" -I"C:/Users/skull/e2_studio/workspace/neural_flow/ra/arm/CMSIS-View/EventRecorder/Config" -std=c99 -Wno-stringop-overflow -Wno-format-truncation -flax-vector-conversions --param=min-pagesize=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"

