################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../CMSIS/EFR32FG1P/startup_gcc_efr32fg1p.s 

C_SRCS += \
../CMSIS/EFR32FG1P/system_efr32fg1p.c 

OBJS += \
./CMSIS/EFR32FG1P/startup_gcc_efr32fg1p.o \
./CMSIS/EFR32FG1P/system_efr32fg1p.o 

C_DEPS += \
./CMSIS/EFR32FG1P/system_efr32fg1p.d 


# Each subdirectory must supply rules for building sources it contributes
CMSIS/EFR32FG1P/%.o: ../CMSIS/EFR32FG1P/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Assembler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -c -x assembler-with-cpp -I"/home/denzo/SimplicityStudio/v4_workspace/devBoardBlinkCPP/inc" -I"/home/denzo/SimplicityStudio/v4_workspace/devBoardBlinkCPP/inc" -I"/home/denzo/Downloads/SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v1.1//hardware/kit/common/bsp" -I"/home/denzo/Downloads/SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v1.1//platform/Device/SiliconLabs/EFR32FG1P/Include" -I"/home/denzo/Downloads/SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v1.1//platform/CMSIS/Include" -I"/home/denzo/Downloads/SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v1.1//platform/emlib/inc" -I"/home/denzo/Downloads/SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v1.1//hardware/kit/common/drivers" '-DEFR32FG1P133F256GM48=1' -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

CMSIS/EFR32FG1P/system_efr32fg1p.o: ../CMSIS/EFR32FG1P/system_efr32fg1p.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-DDEBUG=1' '-DEFR32FG1P133F256GM48=1' -I"/home/denzo/SimplicityStudio/v4_workspace/devBoardBlinkCPP/inc" -I"/home/denzo/SimplicityStudio/v4_workspace/devBoardBlinkCPP/inc" -I"/home/denzo/Downloads/SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v1.1//hardware/kit/common/bsp" -I"/home/denzo/Downloads/SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v1.1//platform/Device/SiliconLabs/EFR32FG1P/Include" -I"/home/denzo/Downloads/SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v1.1//platform/CMSIS/Include" -I"/home/denzo/Downloads/SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v1.1//platform/emlib/inc" -I"/home/denzo/Downloads/SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v1.1//hardware/kit/common/drivers" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -MMD -MP -MF"CMSIS/EFR32FG1P/system_efr32fg1p.d" -MT"CMSIS/EFR32FG1P/system_efr32fg1p.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


