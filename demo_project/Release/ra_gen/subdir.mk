################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ra_gen/common_data.c \
../ra_gen/hal_data.c \
../ra_gen/main.c \
../ra_gen/pin_data.c \
../ra_gen/vector_data.c 

C_DEPS += \
./ra_gen/common_data.d \
./ra_gen/hal_data.d \
./ra_gen/main.d \
./ra_gen/pin_data.d \
./ra_gen/vector_data.d 

OBJS += \
./ra_gen/common_data.o \
./ra_gen/hal_data.o \
./ra_gen/main.o \
./ra_gen/pin_data.o \
./ra_gen/vector_data.o 

SREC += \
neural_flow.srec 

MAP += \
neural_flow.map 


# Each subdirectory must supply rules for building sources it contributes
ra_gen/%.o: ../ra_gen/%.c
	$(file > $@.in,-mthumb -mfloat-abi=hard -mcpu=cortex-m85+nopacbti -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-strict-aliasing -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g -D_RENESAS_RA_ -D_RAFSP_EK_RA8P1_ -D_RA_CORE=CPU0 -D_RA_ORDINAL=1 -I"C:/Users/skull/e2_studio/workspace/neural_flow/mtk3_bsp2" -I"C:/Users/skull/e2_studio/workspace/neural_flow/mtk3_bsp2/mtkernel/kernel/knlinc" -I"C:/Users/skull/e2_studio/workspace/neural_flow/mtk3_bsp2/include" -I"C:/Users/skull/e2_studio/workspace/neural_flow/mtk3_bsp2/config" -I"C:/Users/skull/e2_studio/workspace/neural_flow/ra_cfg/fsp_cfg/bsp" -I"." -I"C:/Users/skull/e2_studio/workspace/neural_flow/ra_gen" -I"C:/Users/skull/e2_studio/workspace/neural_flow/ra_cfg/fsp_cfg" -I"C:/Users/skull/e2_studio/workspace/neural_flow/ra/fsp/inc" -I"C:/Users/skull/e2_studio/workspace/neural_flow/ra/tes/dave2d/inc" -I"C:/Users/skull/e2_studio/workspace/neural_flow/src" -I"C:/Users/skull/e2_studio/workspace/neural_flow/ra/fsp/inc/api" -I"C:/Users/skull/e2_studio/workspace/neural_flow/ra/fsp/inc/instances" -I"C:/Users/skull/e2_studio/workspace/neural_flow/ra/fsp/src/r_drw" -I"C:/Users/skull/e2_studio/workspace/neural_flow/ra/arm/CMSIS_6/CMSIS/Core/Include" -std=c99 -Wno-stringop-overflow -Wno-format-truncation -flax-vector-conversions --param=min-pagesize=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"

