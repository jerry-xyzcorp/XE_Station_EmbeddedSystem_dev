################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../XEstation_Library/Src/User_BeverageManufacturing.c \
../XEstation_Library/Src/User_CupLidDispenser.c \
../XEstation_Library/Src/User_IceDispenser.c 

OBJS += \
./XEstation_Library/Src/User_BeverageManufacturing.o \
./XEstation_Library/Src/User_CupLidDispenser.o \
./XEstation_Library/Src/User_IceDispenser.o 

C_DEPS += \
./XEstation_Library/Src/User_BeverageManufacturing.d \
./XEstation_Library/Src/User_CupLidDispenser.d \
./XEstation_Library/Src/User_IceDispenser.d 


# Each subdirectory must supply rules for building sources it contributes
XEstation_Library/Src/%.o: ../XEstation_Library/Src/%.c XEstation_Library/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Ethan/Documents/stm32/XE_Station_EmbeddedSystem/XEstation_MachineControl/XEstation_Library/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

