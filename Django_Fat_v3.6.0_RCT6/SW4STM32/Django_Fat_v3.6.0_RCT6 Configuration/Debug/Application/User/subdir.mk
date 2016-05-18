################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/Krzysztof/Documents/Studia/Warsztaty\ KNR/Projekty/Django\ Fat/Program/wersja\ 3.6.0\ RCT6/Django_Fat_v3.6.0_RCT6/Src/main.c \
C:/Users/Krzysztof/Documents/Studia/Warsztaty\ KNR/Projekty/Django\ Fat/Program/wersja\ 3.6.0\ RCT6/Django_Fat_v3.6.0_RCT6/Src/stm32f1xx_hal_msp.c \
C:/Users/Krzysztof/Documents/Studia/Warsztaty\ KNR/Projekty/Django\ Fat/Program/wersja\ 3.6.0\ RCT6/Django_Fat_v3.6.0_RCT6/Src/stm32f1xx_it.c 

OBJS += \
./Application/User/main.o \
./Application/User/stm32f1xx_hal_msp.o \
./Application/User/stm32f1xx_it.o 

C_DEPS += \
./Application/User/main.d \
./Application/User/stm32f1xx_hal_msp.d \
./Application/User/stm32f1xx_it.d 


# Each subdirectory must supply rules for building sources it contributes
Application/User/main.o: C:/Users/Krzysztof/Documents/Studia/Warsztaty\ KNR/Projekty/Django\ Fat/Program/wersja\ 3.6.0\ RCT6/Django_Fat_v3.6.0_RCT6/Src/main.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -D__weak="__attribute__((weak))" -D__packed="__attribute__((__packed__))" -DUSE_HAL_DRIVER -DSTM32F103xE -I"C:/Users/Krzysztof/Documents/Studia/Warsztaty KNR/Projekty/Django Fat/Program/wersja 3.6.0 RCT6/Django_Fat_v3.6.0_RCT6/Inc" -I"C:/Users/Krzysztof/Documents/Studia/Warsztaty KNR/Projekty/Django Fat/Program/wersja 3.6.0 RCT6/Django_Fat_v3.6.0_RCT6/Drivers/STM32F1xx_HAL_Driver/Inc" -I"C:/Users/Krzysztof/Documents/Studia/Warsztaty KNR/Projekty/Django Fat/Program/wersja 3.6.0 RCT6/Django_Fat_v3.6.0_RCT6/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Krzysztof/Documents/Studia/Warsztaty KNR/Projekty/Django Fat/Program/wersja 3.6.0 RCT6/Django_Fat_v3.6.0_RCT6/Drivers/CMSIS/Include" -I"C:/Users/Krzysztof/Documents/Studia/Warsztaty KNR/Projekty/Django Fat/Program/wersja 3.6.0 RCT6/Django_Fat_v3.6.0_RCT6/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"C:/Users/Krzysztof/Documents/Studia/Warsztaty KNR/Projekty/Django Fat/Program/wersja 3.6.0 RCT6/Django_Fat_v3.6.0_RCT6/Inc"  -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"Application/User/main.d" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Application/User/stm32f1xx_hal_msp.o: C:/Users/Krzysztof/Documents/Studia/Warsztaty\ KNR/Projekty/Django\ Fat/Program/wersja\ 3.6.0\ RCT6/Django_Fat_v3.6.0_RCT6/Src/stm32f1xx_hal_msp.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -D__weak="__attribute__((weak))" -D__packed="__attribute__((__packed__))" -DUSE_HAL_DRIVER -DSTM32F103xE -I"C:/Users/Krzysztof/Documents/Studia/Warsztaty KNR/Projekty/Django Fat/Program/wersja 3.6.0 RCT6/Django_Fat_v3.6.0_RCT6/Inc" -I"C:/Users/Krzysztof/Documents/Studia/Warsztaty KNR/Projekty/Django Fat/Program/wersja 3.6.0 RCT6/Django_Fat_v3.6.0_RCT6/Drivers/STM32F1xx_HAL_Driver/Inc" -I"C:/Users/Krzysztof/Documents/Studia/Warsztaty KNR/Projekty/Django Fat/Program/wersja 3.6.0 RCT6/Django_Fat_v3.6.0_RCT6/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Krzysztof/Documents/Studia/Warsztaty KNR/Projekty/Django Fat/Program/wersja 3.6.0 RCT6/Django_Fat_v3.6.0_RCT6/Drivers/CMSIS/Include" -I"C:/Users/Krzysztof/Documents/Studia/Warsztaty KNR/Projekty/Django Fat/Program/wersja 3.6.0 RCT6/Django_Fat_v3.6.0_RCT6/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"C:/Users/Krzysztof/Documents/Studia/Warsztaty KNR/Projekty/Django Fat/Program/wersja 3.6.0 RCT6/Django_Fat_v3.6.0_RCT6/Inc"  -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"Application/User/stm32f1xx_hal_msp.d" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Application/User/stm32f1xx_it.o: C:/Users/Krzysztof/Documents/Studia/Warsztaty\ KNR/Projekty/Django\ Fat/Program/wersja\ 3.6.0\ RCT6/Django_Fat_v3.6.0_RCT6/Src/stm32f1xx_it.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -D__weak="__attribute__((weak))" -D__packed="__attribute__((__packed__))" -DUSE_HAL_DRIVER -DSTM32F103xE -I"C:/Users/Krzysztof/Documents/Studia/Warsztaty KNR/Projekty/Django Fat/Program/wersja 3.6.0 RCT6/Django_Fat_v3.6.0_RCT6/Inc" -I"C:/Users/Krzysztof/Documents/Studia/Warsztaty KNR/Projekty/Django Fat/Program/wersja 3.6.0 RCT6/Django_Fat_v3.6.0_RCT6/Drivers/STM32F1xx_HAL_Driver/Inc" -I"C:/Users/Krzysztof/Documents/Studia/Warsztaty KNR/Projekty/Django Fat/Program/wersja 3.6.0 RCT6/Django_Fat_v3.6.0_RCT6/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Krzysztof/Documents/Studia/Warsztaty KNR/Projekty/Django Fat/Program/wersja 3.6.0 RCT6/Django_Fat_v3.6.0_RCT6/Drivers/CMSIS/Include" -I"C:/Users/Krzysztof/Documents/Studia/Warsztaty KNR/Projekty/Django Fat/Program/wersja 3.6.0 RCT6/Django_Fat_v3.6.0_RCT6/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"C:/Users/Krzysztof/Documents/Studia/Warsztaty KNR/Projekty/Django Fat/Program/wersja 3.6.0 RCT6/Django_Fat_v3.6.0_RCT6/Inc"  -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"Application/User/stm32f1xx_it.d" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


