################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/cli/tictactoe.c 

C_DEPS += \
./src/cli/tictactoe.d 

OBJS += \
./src/cli/tictactoe.o 


# Each subdirectory must supply rules for building sources it contributes
src/cli/%.o: ../src/cli/%.c src/cli/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-cli

clean-src-2f-cli:
	-$(RM) ./src/cli/tictactoe.d ./src/cli/tictactoe.o

.PHONY: clean-src-2f-cli

