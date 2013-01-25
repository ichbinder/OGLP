################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ModelObject.cpp \
../src/main.cpp \
../src/paserOBJ.cpp \
../src/shadowmap.cpp 

OBJ_SRCS += \
../src/monkey2.obj \
../src/sto.obj \
../src/world.obj 

OBJS += \
./src/ModelObject.o \
./src/main.o \
./src/paserOBJ.o \
./src/shadowmap.o 

CPP_DEPS += \
./src/ModelObject.d \
./src/main.d \
./src/paserOBJ.d \
./src/shadowmap.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include -I/home/ichbinder/git/OGLP/OGLP/glm-0.9.4.1 -I/usr/include/x86_64-linux-gnu -I/usr/lib/gcc/x86_64-linux-gnu/4.6/include -I/usr/lib/gcc/x86_64-linux-gnu/4.6/include-fixed -I/usr/local/include -I/usr/include/c++/4.6 -I/usr/include/c++/4.6/backward -I/usr/include/c++/4.6/x86_64-linux-gnu -O0 -g3 -fpermissive -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


