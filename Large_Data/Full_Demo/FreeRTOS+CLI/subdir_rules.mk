################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Full_Demo/FreeRTOS+CLI/FreeRTOS_CLI.obj: C:/Users/Gerrit\ van\ der\ Ploeg/Downloads/FreeRTOS/FreeRTOSv10.1.1/FreeRTOS-Plus/Source/FreeRTOS-Plus-CLI/FreeRTOS_CLI.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-msp430_18.1.2.LTS/bin/cl430" -vmspx --abi=eabi --data_model=large -O0 --opt_for_speed=1 --use_hw_mpy=F5 --include_path="C:/ti/ccsv8/ccs_base/msp430/include" --include_path="C:/Users/Gerrit van der Ploeg/Downloads/FreeRTOS/FreeRTOSv10.1.1/FreeRTOS/Demo/MSP430X_MSP430FR5969_LaunchPad_IAR_CCS" --include_path="C:/Users/Gerrit van der Ploeg/Downloads/FreeRTOS/FreeRTOSv10.1.1/FreeRTOS/Source/include" --include_path="C:/Users/Gerrit van der Ploeg/Downloads/FreeRTOS/FreeRTOSv10.1.1/FreeRTOS/Source/portable/CCS/MSP430X" --include_path="C:/Users/Gerrit van der Ploeg/Downloads/FreeRTOS/FreeRTOSv10.1.1/FreeRTOS-Plus/Source/FreeRTOS-Plus-CLI" --include_path="C:/Users/Gerrit van der Ploeg/Downloads/FreeRTOS/FreeRTOSv10.1.1/FreeRTOS/Demo/Common/include" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-msp430_18.1.2.LTS/include" --include_path="C:/Users/Gerrit van der Ploeg/Downloads/FreeRTOS/FreeRTOSv10.1.1/FreeRTOS/Demo/MSP430X_MSP430FR5969_LaunchPad_IAR_CCS/driverlib/MSP430FR5xx_6xx" -g --define=__MSP430FR5969__ --display_error_number --diag_warning=225 --diag_wrap=off --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=full --preproc_with_compile --preproc_dependency="Full_Demo/FreeRTOS+CLI/FreeRTOS_CLI.d_raw" --obj_directory="Full_Demo/FreeRTOS+CLI" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


