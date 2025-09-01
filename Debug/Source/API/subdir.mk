################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Source/API/accel_api.c \
../Source/API/cli_handler.c \
../Source/API/cmd_api.c \
../Source/API/gnss_api.c \
../Source/API/gnss_handler.c \
../Source/API/heap_api.c \
../Source/API/modem_api.c \
../Source/API/modem_cmd_handler.c \
../Source/API/tcp_api.c \
../Source/API/uart_api.c 

OBJS += \
./Source/API/accel_api.o \
./Source/API/cli_handler.o \
./Source/API/cmd_api.o \
./Source/API/gnss_api.o \
./Source/API/gnss_handler.o \
./Source/API/heap_api.o \
./Source/API/modem_api.o \
./Source/API/modem_cmd_handler.o \
./Source/API/tcp_api.o \
./Source/API/uart_api.o 

C_DEPS += \
./Source/API/accel_api.d \
./Source/API/cli_handler.d \
./Source/API/cmd_api.d \
./Source/API/gnss_api.d \
./Source/API/gnss_handler.d \
./Source/API/heap_api.d \
./Source/API/modem_api.d \
./Source/API/modem_cmd_handler.d \
./Source/API/tcp_api.d \
./Source/API/uart_api.d 


# Each subdirectory must supply rules for building sources it contributes
Source/API/%.o Source/API/%.su Source/API/%.cyclo: ../Source/API/%.c Source/API/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xE -DUSE_FULL_LL_DRIVER -c -I../Source/APP -I../Source/API -I../Source/Driver -I../Source/ThirdParty/Core/Inc -I../Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Inc -I../Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Source/ThirdParty/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Source/ThirdParty/Drivers/CMSIS/Include -I../Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/include -I../Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Source/Utility -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Source-2f-API

clean-Source-2f-API:
	-$(RM) ./Source/API/accel_api.cyclo ./Source/API/accel_api.d ./Source/API/accel_api.o ./Source/API/accel_api.su ./Source/API/cli_handler.cyclo ./Source/API/cli_handler.d ./Source/API/cli_handler.o ./Source/API/cli_handler.su ./Source/API/cmd_api.cyclo ./Source/API/cmd_api.d ./Source/API/cmd_api.o ./Source/API/cmd_api.su ./Source/API/gnss_api.cyclo ./Source/API/gnss_api.d ./Source/API/gnss_api.o ./Source/API/gnss_api.su ./Source/API/gnss_handler.cyclo ./Source/API/gnss_handler.d ./Source/API/gnss_handler.o ./Source/API/gnss_handler.su ./Source/API/heap_api.cyclo ./Source/API/heap_api.d ./Source/API/heap_api.o ./Source/API/heap_api.su ./Source/API/modem_api.cyclo ./Source/API/modem_api.d ./Source/API/modem_api.o ./Source/API/modem_api.su ./Source/API/modem_cmd_handler.cyclo ./Source/API/modem_cmd_handler.d ./Source/API/modem_cmd_handler.o ./Source/API/modem_cmd_handler.su ./Source/API/tcp_api.cyclo ./Source/API/tcp_api.d ./Source/API/tcp_api.o ./Source/API/tcp_api.su ./Source/API/uart_api.cyclo ./Source/API/uart_api.d ./Source/API/uart_api.o ./Source/API/uart_api.su

.PHONY: clean-Source-2f-API

