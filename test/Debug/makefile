################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: test

dependents:
	-cd ../../so-commons-library/Debug && $(MAKE) clean all
	-cd ../../nivel-gui/Debug && $(MAKE) clean all

# Tool invocations
test: $(OBJS) $(USER_OBJS) ../../so-commons-library/Debug/libso-commons-library.so ../../nivel-gui/Debug/libnivel-gui.so
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc -L"../../nivel-gui/Debug" -L"../../so-commons-library/Debug" -o "test" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) test
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:
../../so-commons-library/Debug/libso-commons-library.so:
../../nivel-gui/Debug/libnivel-gui.so:

-include ../makefile.targets
