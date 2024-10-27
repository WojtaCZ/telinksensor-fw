################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 

OBJS += \
$(OUT_PATH)/main.o \
$(OUT_PATH)/app_uart.o \
$(OUT_PATH)/bmp390.o \
$(OUT_PATH)/sth40.o\
$(OUT_PATH)/voltages.o \
$(OUT_PATH)/rtd.o 

# Each subdirectory must supply rules for building sources it contributes
$(OUT_PATH)/%.o: ./%.c
	@echo 'Building file: $<'
	@tc32-elf-gcc $(GCC_FLAGS) $(INCLUDE_PATHS) -c -o"$@" "$<"



 