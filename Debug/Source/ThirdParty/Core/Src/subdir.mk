################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Source/ThirdParty/Core/Src/freertos.c \
../Source/ThirdParty/Core/Src/stm32f4xx_hal_msp.c \
../Source/ThirdParty/Core/Src/stm32f4xx_hal_timebase_tim.c \
../Source/ThirdParty/Core/Src/stm32f4xx_it.c \
../Source/ThirdParty/Core/Src/syscalls.c \
../Source/ThirdParty/Core/Src/sysmem.c \
../Source/ThirdParty/Core/Src/system_stm32f4xx.c 

OBJS += \
./Source/ThirdParty/Core/Src/freertos.o \
./Source/ThirdParty/Core/Src/stm32f4xx_hal_msp.o \
./Source/ThirdParty/Core/Src/stm32f4xx_hal_timebase_tim.o \
./Source/ThirdParty/Core/Src/stm32f4xx_it.o \
./Source/ThirdParty/Core/Src/syscalls.o \
./Source/ThirdParty/Core/Src/sysmem.o \
./Source/ThirdParty/Core/Src/system_stm32f4xx.o 

C_DEPS += \
./Source/ThirdParty/Core/Src/freertos.d \
./Source/ThirdParty/Core/Src/stm32f4xx_hal_msp.d \
./Source/ThirdParty/Core/Src/stm32f4xx_hal_timebase_tim.d \
./Source/ThirdParty/Core/Src/stm32f4xx_it.d \
./Source/ThirdParty/Core/Src/syscalls.d \
./Source/ThirdParty/Core/Src/sysmem.d \
./Source/ThirdParty/Core/Src/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Source/ThirdParty/Core/Src/%.o Source/ThirdParty/Core/Src/%.su Source/ThirdParty/Core/Src/%.cyclo: ../Source/ThirdParty/Core/Src/%.c Source/ThirdParty/Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xE -DUSE_FULL_LL_DRIVER -c -I../Source/APP -I../Source/API -I../Source/Driver -I../Source/ThirdParty/Core/Inc -I../Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Inc -I../Source/ThirdParty/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Source/ThirdParty/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Source/ThirdParty/Drivers/CMSIS/Include -I../Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/include -I../Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Source/ThirdParty/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Source/Utility -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Source-2f-ThirdParty-2f-Core-2f-Src

clean-Source-2f-ThirdParty-2f-Core-2f-Src:
	-$(RM) ./Source/ThirdParty/Core/Src/freertos.cyclo ./Source/ThirdParty/Core/Src/freertos.d ./Source/ThirdParty/Core/Src/freertos.o ./Source/ThirdParty/Core/Src/freertos.su ./Source/ThirdParty/Core/Src/stm32f4xx_hal_msp.cyclo ./Source/ThirdParty/Core/Src/stm32f4xx_hal_msp.d ./Source/ThirdParty/Core/Src/stm32f4xx_hal_msp.o ./Source/ThirdParty/Core/Src/stm32f4xx_hal_msp.su ./Source/ThirdParty/Core/Src/stm32f4xx_hal_timebase_tim.cyclo ./Source/ThirdParty/Core/Src/stm32f4xx_hal_timebase_tim.d ./Source/ThirdParty/Core/Src/stm32f4xx_hal_timebase_tim.o ./Source/ThirdParty/Core/Src/stm32f4xx_hal_timebase_tim.su ./Source/ThirdParty/Core/Src/stm32f4xx_it.cyclo ./Source/ThirdParty/Core/Src/stm32f4xx_it.d ./Source/ThirdParty/Core/Src/stm32f4xx_it.o ./Source/ThirdParty/Core/Src/stm32f4xx_it.su ./Source/ThirdParty/Core/Src/syscalls.cyclo ./Source/ThirdParty/Core/Src/syscalls.d ./Source/ThirdParty/Core/Src/syscalls.o ./Source/ThirdParty/Core/Src/syscalls.su ./Source/ThirdParty/Core/Src/sysmem.cyclo ./Source/ThirdParty/Core/Src/sysmem.d ./Source/ThirdParty/Core/Src/sysmem.o ./Source/ThirdParty/Core/Src/sysmem.su ./Source/ThirdParty/Core/Src/system_stm32f4xx.cyclo ./Source/ThirdParty/Core/Src/system_stm32f4xx.d ./Source/ThirdParty/Core/Src/system_stm32f4xx.o ./Source/ThirdParty/Core/Src/system_stm32f4xx.su

.PHONY: clean-Source-2f-ThirdParty-2f-Core-2f-Src

