################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/NV-Smart03/Downloads/sdk_zigbee_v3.0/zigbee1.0/zigbee/zcl/smart_energy/zcl_metering.c \
C:/Users/NV-Smart03/Downloads/sdk_zigbee_v3.0/zigbee1.0/zigbee/zcl/smart_energy/zcl_metering_attr.c 

OBJS += \
./zigbee/zcl/smart_energy/zcl_metering.o \
./zigbee/zcl/smart_energy/zcl_metering_attr.o 


# Each subdirectory must supply rules for building sources it contributes
zigbee/zcl/smart_energy/zcl_metering.o: C:/Users/NV-Smart03/Downloads/sdk_zigbee_v3.0/zigbee1.0/zigbee/zcl/smart_energy/zcl_metering.c
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -I../../../apps/common -I../../../apps/sampleContactSensor -I../../../platform -I../../../proj/common -I../../../proj -I../../../zigbee/common/includes -I../../../zigbee/zbapi -I../../../zigbee/bdb/includes -I../../../zigbee/zcl -I../../../zigbee/ota -I../../../zbhci -DEND_DEVICE=1 -DMCU_CORE_8258=1 -D__PROJECT_TL_CONTACT_SENSOR__=1 -Wall -O2 -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

zigbee/zcl/smart_energy/zcl_metering_attr.o: C:/Users/NV-Smart03/Downloads/sdk_zigbee_v3.0/zigbee1.0/zigbee/zcl/smart_energy/zcl_metering_attr.c
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -I../../../apps/common -I../../../apps/sampleContactSensor -I../../../platform -I../../../proj/common -I../../../proj -I../../../zigbee/common/includes -I../../../zigbee/zbapi -I../../../zigbee/bdb/includes -I../../../zigbee/zcl -I../../../zigbee/ota -I../../../zbhci -DEND_DEVICE=1 -DMCU_CORE_8258=1 -D__PROJECT_TL_CONTACT_SENSOR__=1 -Wall -O2 -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


