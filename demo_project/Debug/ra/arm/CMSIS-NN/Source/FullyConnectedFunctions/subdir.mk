################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ra/arm/CMSIS-NN/Source/FullyConnectedFunctions/arm_batch_matmul_s16.c \
../ra/arm/CMSIS-NN/Source/FullyConnectedFunctions/arm_batch_matmul_s8.c \
../ra/arm/CMSIS-NN/Source/FullyConnectedFunctions/arm_fully_connected_get_buffer_sizes_s16.c \
../ra/arm/CMSIS-NN/Source/FullyConnectedFunctions/arm_fully_connected_get_buffer_sizes_s8.c \
../ra/arm/CMSIS-NN/Source/FullyConnectedFunctions/arm_fully_connected_per_channel_s8.c \
../ra/arm/CMSIS-NN/Source/FullyConnectedFunctions/arm_fully_connected_s16.c \
../ra/arm/CMSIS-NN/Source/FullyConnectedFunctions/arm_fully_connected_s4.c \
../ra/arm/CMSIS-NN/Source/FullyConnectedFunctions/arm_fully_connected_s8.c \
../ra/arm/CMSIS-NN/Source/FullyConnectedFunctions/arm_fully_connected_wrapper_s8.c \
../ra/arm/CMSIS-NN/Source/FullyConnectedFunctions/arm_vector_sum_s8.c \
../ra/arm/CMSIS-NN/Source/FullyConnectedFunctions/arm_vector_sum_s8_s64.c 

C_DEPS += \
./ra/arm/CMSIS-NN/Source/FullyConnectedFunctions/arm_batch_matmul_s16.d \
./ra/arm/CMSIS-NN/Source/FullyConnectedFunctions/arm_batch_matmul_s8.d \
./ra/arm/CMSIS-NN/Source/FullyConnectedFunctions/arm_fully_connected_get_buffer_sizes_s16.d \
./ra/arm/CMSIS-NN/Source/FullyConnectedFunctions/arm_fully_connected_get_buffer_sizes_s8.d \
./ra/arm/CMSIS-NN/Source/FullyConnectedFunctions/arm_fully_connected_per_channel_s8.d \
./ra/arm/CMSIS-NN/Source/FullyConnectedFunctions/arm_fully_connected_s16.d \
./ra/arm/CMSIS-NN/Source/FullyConnectedFunctions/arm_fully_connected_s4.d \
./ra/arm/CMSIS-NN/Source/FullyConnectedFunctions/arm_fully_connected_s8.d \
./ra/arm/CMSIS-NN/Source/FullyConnectedFunctions/arm_fully_connected_wrapper_s8.d \
./ra/arm/CMSIS-NN/Source/FullyConnectedFunctions/arm_vector_sum_s8.d \
./ra/arm/CMSIS-NN/Source/FullyConnectedFunctions/arm_vector_sum_s8_s64.d 

OBJS += \
./ra/arm/CMSIS-NN/Source/FullyConnectedFunctions/arm_batch_matmul_s16.o \
./ra/arm/CMSIS-NN/Source/FullyConnectedFunctions/arm_batch_matmul_s8.o \
./ra/arm/CMSIS-NN/Source/FullyConnectedFunctions/arm_fully_connected_get_buffer_sizes_s16.o \
./ra/arm/CMSIS-NN/Source/FullyConnectedFunctions/arm_fully_connected_get_buffer_sizes_s8.o \
./ra/arm/CMSIS-NN/Source/FullyConnectedFunctions/arm_fully_connected_per_channel_s8.o \
./ra/arm/CMSIS-NN/Source/FullyConnectedFunctions/arm_fully_connected_s16.o \
./ra/arm/CMSIS-NN/Source/FullyConnectedFunctions/arm_fully_connected_s4.o \
./ra/arm/CMSIS-NN/Source/FullyConnectedFunctions/arm_fully_connected_s8.o \
./ra/arm/CMSIS-NN/Source/FullyConnectedFunctions/arm_fully_connected_wrapper_s8.o \
./ra/arm/CMSIS-NN/Source/FullyConnectedFunctions/arm_vector_sum_s8.o \
./ra/arm/CMSIS-NN/Source/FullyConnectedFunctions/arm_vector_sum_s8_s64.o 

SREC += \
neural_flow.srec 

MAP += \
neural_flow.map 


# Each subdirectory must supply rules for building sources it contributes
ra/arm/CMSIS-NN/Source/FullyConnectedFunctions/%.o: ../ra/arm/CMSIS-NN/Source/FullyConnectedFunctions/%.c
	$(file > $@.in,-mthumb -mfloat-abi=hard -mcpu=cortex-m85+nopacbti -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-strict-aliasing -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g -D_RENESAS_RA_ -D_RAFSP_EK_RA8P1_ -D_RA_CORE=CPU0 -D_RA_ORDINAL=1 -I"C:/Users/skull/e2_studio/workspace/neural_flow/ra_cfg/fsp_cfg/bsp" -I"C:/Users/skull/e2_studio/workspace/neural_flow/mtk3_bsp2" -I"C:/Users/skull/e2_studio/workspace/neural_flow/mtk3_bsp2/config" -I"C:/Users/skull/e2_studio/workspace/neural_flow/mtk3_bsp2/include" -I"C:/Users/skull/e2_studio/workspace/neural_flow/mtk3_bsp2/mtkernel/kernel/knlinc" -I"." -I"C:/Users/skull/e2_studio/workspace/neural_flow/ra_gen" -I"C:/Users/skull/e2_studio/workspace/neural_flow/ra_cfg/fsp_cfg" -I"C:/Users/skull/e2_studio/workspace/neural_flow/ra/fsp/inc" -I"C:/Users/skull/e2_studio/workspace/neural_flow/ra/tes/dave2d/inc" -I"C:/Users/skull/e2_studio/workspace/neural_flow/src" -I"C:/Users/skull/e2_studio/workspace/neural_flow/ra/fsp/inc/api" -I"C:/Users/skull/e2_studio/workspace/neural_flow/ra/fsp/inc/instances" -I"C:/Users/skull/e2_studio/workspace/neural_flow/ra/fsp/src/r_drw" -I"C:/Users/skull/e2_studio/workspace/neural_flow/ra/arm/CMSIS_6/CMSIS/Core/Include" -I"C:/Users/skull/e2_studio/workspace/neural_flow/ra/arm/CMSIS-NN/Include" -I"C:/Users/skull/e2_studio/workspace/neural_flow/ra/arm/CMSIS-NN" -I"C:/Users/skull/e2_studio/workspace/neural_flow/ra/arm/CMSIS-View/EventRecorder/Include" -I"C:/Users/skull/e2_studio/workspace/neural_flow/ra/arm/CMSIS-View/EventRecorder/Config" -std=c99 -Wno-stringop-overflow -Wno-format-truncation -w -flax-vector-conversions --param=min-pagesize=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"

