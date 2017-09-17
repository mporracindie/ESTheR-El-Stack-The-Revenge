################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Kernel.c 

OBJS += \
./src/Kernel.o 

C_DEPS += \
./src/Kernel.d 


# Each subdirectory must supply rules for building sources it contributes
src/Kernel.o: ../src/Kernel.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/workspace/tp-2017-1c-AsadoClash/Libraries" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"src/Kernel.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


