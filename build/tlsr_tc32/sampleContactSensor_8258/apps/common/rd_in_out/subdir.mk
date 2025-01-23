################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/NV-Smart03/Downloads/sdk_zigbee_v3.0/zigbee1.0/apps/common/rd_in_out/rd_cluster.c \
C:/Users/NV-Smart03/Downloads/sdk_zigbee_v3.0/zigbee1.0/apps/common/rd_in_out/rd_in_out.c \
C:/Users/NV-Smart03/Downloads/sdk_zigbee_v3.0/zigbee1.0/apps/common/rd_in_out/rd_pir.c \
C:/Users/NV-Smart03/Downloads/sdk_zigbee_v3.0/zigbee1.0/apps/common/rd_in_out/rd_pyd_1598.c 

OBJS += \
./apps/common/rd_in_out/rd_cluster.o \
./apps/common/rd_in_out/rd_in_out.o \
./apps/common/rd_in_out/rd_pir.o \
./apps/common/rd_in_out/rd_pyd_1598.o 


# Each subdirectory must supply rules for building sources it contributes
apps/common/rd_in_out/rd_cluster.o: C:/Users/NV-Smart03/Downloads/sdk_zigbee_v3.0/zigbee1.0/apps/common/rd_in_out/rd_cluster.c
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -I../../../apps/common -I../../../apps/sampleContactSensor -I../../../platform -I../../../proj/common -I../../../proj -I../../../zigbee/common/includes -I../../../zigbee/zbapi -I../../../zigbee/bdb/includes -I../../../zigbee/zcl -I../../../zigbee/ota -I../../../zbhci -DEND_DEVICE=1 -DMCU_CORE_8258=1 -D__PROJECT_TL_CONTACT_SENSOR__=1 -Wall -O2 -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

apps/common/rd_in_out/rd_in_out.o: C:/Users/NV-Smart03/Downloads/sdk_zigbee_v3.0/zigbee1.0/apps/common/rd_in_out/rd_in_out.c
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -I../../../apps/common -I../../../apps/sampleContactSensor -I../../../platform -I../../../proj/common -I../../../proj -I../../../zigbee/common/includes -I../../../zigbee/zbapi -I../../../zigbee/bdb/includes -I../../../zigbee/zcl -I../../../zigbee/ota -I../../../zbhci -DEND_DEVICE=1 -DMCU_CORE_8258=1 -D__PROJECT_TL_CONTACT_SENSOR__=1 -Wall -O2 -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

apps/common/rd_in_out/rd_pir.o: C:/Users/NV-Smart03/Downloads/sdk_zigbee_v3.0/zigbee1.0/apps/common/rd_in_out/rd_pir.c
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -I../../../apps/common -I../../../apps/sampleContactSensor -I../../../platform -I../../../proj/common -I../../../proj -I../../../zigbee/common/includes -I../../../zigbee/zbapi -I../../../zigbee/bdb/includes -I../../../zigbee/zcl -I../../../zigbee/ota -I../../../zbhci -DEND_DEVICE=1 -DMCU_CORE_8258=1 -D__PROJECT_TL_CONTACT_SENSOR__=1 -Wall -O2 -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

apps/common/rd_in_out/rd_pyd_1598.o: C:/Users/NV-Smart03/Downloads/sdk_zigbee_v3.0/zigbee1.0/apps/common/rd_in_out/rd_pyd_1598.c
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -I../../../apps/common -I../../../apps/sampleContactSensor -I../../../platform -I../../../proj/common -I../../../proj -I../../../zigbee/common/includes -I../../../zigbee/zbapi -I../../../zigbee/bdb/includes -I../../../zigbee/zcl -I../../../zigbee/ota -I../../../zbhci -DEND_DEVICE=1 -DMCU_CORE_8258=1 -D__PROJECT_TL_CONTACT_SENSOR__=1 -Wall -O2 -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


