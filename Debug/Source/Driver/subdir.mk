################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Source/Driver/gpio_driver.c \
../Source/Driver/i2c_driver.c \
../Source/Driver/uart_driver.c 

OBJS += \
./Source/Driver/gpio_driver.o \
./Source/Driver/i2c_driver.o \
./Source/Driver/uart_driver.o 

C_DEPS += \
./Source/Driver/gpio_driver.d \
./Source/Driver/i2c_driver.d \
./Source/Driver/uart_driver.d 


# Each subdirectory must supply rules for building sources it contributes
Source/Driver/%.o Source/Driver/%.su Source/Driver/%.cyclo: ../Source/Driver/%.c Source/Driver/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xE -DUSE_FULL_LL_DRIVER -c -I../Source/APP -I../Source/API -I../Source/Driver -I../Source/ThirdParty/Core/Inc -I../Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Inc -I../Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Source/ThirdParty/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Source/ThirdParty/Drivers/CMSIS/Include -I../Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/include -I../Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Source/Utility -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Source-2f-Driver

clean-Source-2f-Driver:
	-$(RM) ./Source/Driver/gpio_driver.cyclo ./Source/Driver/gpio_driver.d ./Source/Driver/gpio_driver.o ./Source/Driver/gpio_driver.su ./Source/Driver/i2c_driver.cyclo ./Source/Driver/i2c_driver.d ./Source/Driver/i2c_driver.o ./Source/Driver/i2c_driver.su ./Source/Driver/uart_driver.cyclo ./Source/Driver/uart_driver.d ./Source/Driver/uart_driver.o ./Source/Driver/uart_driver.su

.PHONY: clean-Source-2f-Driver

