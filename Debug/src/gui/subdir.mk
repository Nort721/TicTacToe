################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/gui/engine.c \
../src/gui/uiapi.c 

C_DEPS += \
./src/gui/engine.d \
./src/gui/uiapi.d 

OBJS += \
./src/gui/engine.o \
./src/gui/uiapi.o 


# Each subdirectory must supply rules for building sources it contributes
src/gui/%.o: ../src/gui/%.c src/gui/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-gui

clean-src-2f-gui:
	-$(RM) ./src/gui/engine.d ./src/gui/engine.o ./src/gui/uiapi.d ./src/gui/uiapi.o

.PHONY: clean-src-2f-gui

