################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/Clase_10_11.c \
../source/Led_and_switch_control.c \
../source/mtb.c \
../source/semihost_hardfault.c 

OBJS += \
./source/Clase_10_11.o \
./source/Led_and_switch_control.o \
./source/mtb.o \
./source/semihost_hardfault.o 

C_DEPS += \
./source/Clase_10_11.d \
./source/Led_and_switch_control.d \
./source/mtb.d \
./source/semihost_hardfault.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MKL43Z256VLH4 -DCPU_MKL43Z256VLH4_cm0plus -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSERIAL_PORT_TYPE_UART=1 -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DNDEBUG -D__REDLIB__ -I"C:\Users\lraff\Documents\MCUXpressoIDE_11.5.1_7266\workspace\Clase_10_11\board" -I"C:\Users\lraff\Documents\MCUXpressoIDE_11.5.1_7266\workspace\Clase_10_11\source" -I"C:\Users\lraff\Documents\MCUXpressoIDE_11.5.1_7266\workspace\Clase_10_11" -I"C:\Users\lraff\Documents\MCUXpressoIDE_11.5.1_7266\workspace\Clase_10_11\drivers" -I"C:\Users\lraff\Documents\MCUXpressoIDE_11.5.1_7266\workspace\Clase_10_11\component\uart" -I"C:\Users\lraff\Documents\MCUXpressoIDE_11.5.1_7266\workspace\Clase_10_11\component\serial_manager" -I"C:\Users\lraff\Documents\MCUXpressoIDE_11.5.1_7266\workspace\Clase_10_11\utilities" -I"C:\Users\lraff\Documents\MCUXpressoIDE_11.5.1_7266\workspace\Clase_10_11\component\lists" -I"C:\Users\lraff\Documents\MCUXpressoIDE_11.5.1_7266\workspace\Clase_10_11\device" -I"C:\Users\lraff\Documents\MCUXpressoIDE_11.5.1_7266\workspace\Clase_10_11\CMSIS" -Os -fno-common -g -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


