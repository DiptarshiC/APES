################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
main.obj: ../main.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"D:/Code Composer Studio/ccsv8/tools/compiler/ti-cgt-arm_18.1.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="D:/APES/HW5" --include_path="D:/Code Composer Studio/ccsv8/tools/compiler/ti-cgt-arm_18.1.1.LTS/include" --define=ccs="ccs" --define=PART_TM4C1292NCPDT -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="main.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

tm4c1292ncpdt_startup_ccs.obj: ../tm4c1292ncpdt_startup_ccs.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"D:/Code Composer Studio/ccsv8/tools/compiler/ti-cgt-arm_18.1.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="D:/APES/HW5" --include_path="D:/Code Composer Studio/ccsv8/tools/compiler/ti-cgt-arm_18.1.1.LTS/include" --define=ccs="ccs" --define=PART_TM4C1292NCPDT -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="tm4c1292ncpdt_startup_ccs.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


