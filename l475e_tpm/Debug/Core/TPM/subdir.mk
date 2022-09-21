################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/TPM/authhandler.c \
../Core/TPM/commandFuncs.c \
../Core/TPM/commandstructs.c \
../Core/TPM/globals.c \
../Core/TPM/hashfuncs.c \
../Core/TPM/nvstorefuncs.c \
../Core/TPM/physical.c \
../Core/TPM/tpm_demo.c \
../Core/TPM/utils.c 

OBJS += \
./Core/TPM/authhandler.o \
./Core/TPM/commandFuncs.o \
./Core/TPM/commandstructs.o \
./Core/TPM/globals.o \
./Core/TPM/hashfuncs.o \
./Core/TPM/nvstorefuncs.o \
./Core/TPM/physical.o \
./Core/TPM/tpm_demo.o \
./Core/TPM/utils.o 

C_DEPS += \
./Core/TPM/authhandler.d \
./Core/TPM/commandFuncs.d \
./Core/TPM/commandstructs.d \
./Core/TPM/globals.d \
./Core/TPM/hashfuncs.d \
./Core/TPM/nvstorefuncs.d \
./Core/TPM/physical.d \
./Core/TPM/tpm_demo.d \
./Core/TPM/utils.d 


# Each subdirectory must supply rules for building sources it contributes
Core/TPM/%.o Core/TPM/%.su: ../Core/TPM/%.c Core/TPM/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L475xx -DSPI_INTERFACE -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Core/TPM -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-TPM

clean-Core-2f-TPM:
	-$(RM) ./Core/TPM/authhandler.d ./Core/TPM/authhandler.o ./Core/TPM/authhandler.su ./Core/TPM/commandFuncs.d ./Core/TPM/commandFuncs.o ./Core/TPM/commandFuncs.su ./Core/TPM/commandstructs.d ./Core/TPM/commandstructs.o ./Core/TPM/commandstructs.su ./Core/TPM/globals.d ./Core/TPM/globals.o ./Core/TPM/globals.su ./Core/TPM/hashfuncs.d ./Core/TPM/hashfuncs.o ./Core/TPM/hashfuncs.su ./Core/TPM/nvstorefuncs.d ./Core/TPM/nvstorefuncs.o ./Core/TPM/nvstorefuncs.su ./Core/TPM/physical.d ./Core/TPM/physical.o ./Core/TPM/physical.su ./Core/TPM/tpm_demo.d ./Core/TPM/tpm_demo.o ./Core/TPM/tpm_demo.su ./Core/TPM/utils.d ./Core/TPM/utils.o ./Core/TPM/utils.su

.PHONY: clean-Core-2f-TPM

