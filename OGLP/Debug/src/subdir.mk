################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ModelObject.cpp \
../src/main.cpp \
../src/paserOBJ.cpp 

OBJS += \
./src/ModelObject.o \
./src/main.o \
./src/paserOBJ.o 

CPP_DEPS += \
./src/ModelObject.d \
./src/main.d \
./src/paserOBJ.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


