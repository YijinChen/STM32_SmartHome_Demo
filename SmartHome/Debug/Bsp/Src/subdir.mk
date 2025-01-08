################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Bsp/Src/bh1750.c \
../Bsp/Src/cJSON.c \
../Bsp/Src/dht11.c \
../Bsp/Src/oled.c 

OBJS += \
./Bsp/Src/bh1750.o \
./Bsp/Src/cJSON.o \
./Bsp/Src/dht11.o \
./Bsp/Src/oled.o 

C_DEPS += \
./Bsp/Src/bh1750.d \
./Bsp/Src/cJSON.d \
./Bsp/Src/dht11.d \
./Bsp/Src/oled.d 


# Each subdirectory must supply rules for building sources it contributes
Bsp/Src/%.o Bsp/Src/%.su Bsp/Src/%.cyclo: ../Bsp/Src/%.c Bsp/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I/Users/yijin/Documents/Cpp_Projects/IoT_Project/SmartHome/Bsp/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Bsp-2f-Src

clean-Bsp-2f-Src:
	-$(RM) ./Bsp/Src/bh1750.cyclo ./Bsp/Src/bh1750.d ./Bsp/Src/bh1750.o ./Bsp/Src/bh1750.su ./Bsp/Src/cJSON.cyclo ./Bsp/Src/cJSON.d ./Bsp/Src/cJSON.o ./Bsp/Src/cJSON.su ./Bsp/Src/dht11.cyclo ./Bsp/Src/dht11.d ./Bsp/Src/dht11.o ./Bsp/Src/dht11.su ./Bsp/Src/oled.cyclo ./Bsp/Src/oled.d ./Bsp/Src/oled.o ./Bsp/Src/oled.su

.PHONY: clean-Bsp-2f-Src

