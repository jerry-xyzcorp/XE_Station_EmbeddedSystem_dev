################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../XEstation_Library/Src/User_HC_SR04.c \
../XEstation_Library/Src/User_Lidar.c \
../XEstation_Library/Src/User_PC_serial.c \
../XEstation_Library/Src/User_TFminiS.c \
../XEstation_Library/Src/User_hx711.c \
../XEstation_Library/Src/hx711.c 

OBJS += \
./XEstation_Library/Src/User_HC_SR04.o \
./XEstation_Library/Src/User_Lidar.o \
./XEstation_Library/Src/User_PC_serial.o \
./XEstation_Library/Src/User_TFminiS.o \
./XEstation_Library/Src/User_hx711.o \
./XEstation_Library/Src/hx711.o 

C_DEPS += \
./XEstation_Library/Src/User_HC_SR04.d \
./XEstation_Library/Src/User_Lidar.d \
./XEstation_Library/Src/User_PC_serial.d \
./XEstation_Library/Src/User_TFminiS.d \
./XEstation_Library/Src/User_hx711.d \
./XEstation_Library/Src/hx711.d 


# Each subdirectory must supply rules for building sources it contributes
XEstation_Library/Src/%.o: ../XEstation_Library/Src/%.c XEstation_Library/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/gad05/Documents/XE_station/XE_Station_EmbeddedSystem_dev/XEstation_InventoryManagement/XEstation_Library/Inc" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

