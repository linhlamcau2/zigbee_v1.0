################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/NV-Smart03/Downloads/sdk_zigbee_v3.0/zigbee1.0/zigbee/zcl/security_safety/zcl_ias_ace.c \
C:/Users/NV-Smart03/Downloads/sdk_zigbee_v3.0/zigbee1.0/zigbee/zcl/security_safety/zcl_ias_wd.c \
C:/Users/NV-Smart03/Downloads/sdk_zigbee_v3.0/zigbee1.0/zigbee/zcl/security_safety/zcl_ias_wd_attr.c \
C:/Users/NV-Smart03/Downloads/sdk_zigbee_v3.0/zigbee1.0/zigbee/zcl/security_safety/zcl_ias_zone.c \
C:/Users/NV-Smart03/Downloads/sdk_zigbee_v3.0/zigbee1.0/zigbee/zcl/security_safety/zcl_ias_zone_attr.c 

OBJS += \
./zigbee/zcl/security_safety/zcl_ias_ace.o \
./zigbee/zcl/security_safety/zcl_ias_wd.o \
./zigbee/zcl/security_safety/zcl_ias_wd_attr.o \
./zigbee/zcl/security_safety/zcl_ias_zone.o \
./zigbee/zcl/security_safety/zcl_ias_zone_attr.o 


# Each subdirectory must supply rules for building sources it contributes
zigbee/zcl/security_safety/zcl_ias_ace.o: C:/Users/NV-Smart03/Downloads/sdk_zigbee_v3.0/zigbee1.0/zigbee/zcl/security_safety/zcl_ias_ace.c
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -I../../../apps/common -I../../../apps/sampleLight -I../../../platform -I../../../proj/common -I../../../proj -I../../../zigbee/common/includes -I../../../zigbee/zbapi -I../../../zigbee/bdb/includes -I../../../zigbee/gp -I../../../zigbee/zcl -I../../../zigbee/ota -I../../../zigbee/wwah -I../../../zbhci -DROUTER=1 -DMCU_CORE_8258=1 -D__PROJECT_TL_DIMMABLE_LIGHT__=1 -Wall -O2 -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

zigbee/zcl/security_safety/zcl_ias_wd.o: C:/Users/NV-Smart03/Downloads/sdk_zigbee_v3.0/zigbee1.0/zigbee/zcl/security_safety/zcl_ias_wd.c
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -I../../../apps/common -I../../../apps/sampleLight -I../../../platform -I../../../proj/common -I../../../proj -I../../../zigbee/common/includes -I../../../zigbee/zbapi -I../../../zigbee/bdb/includes -I../../../zigbee/gp -I../../../zigbee/zcl -I../../../zigbee/ota -I../../../zigbee/wwah -I../../../zbhci -DROUTER=1 -DMCU_CORE_8258=1 -D__PROJECT_TL_DIMMABLE_LIGHT__=1 -Wall -O2 -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

zigbee/zcl/security_safety/zcl_ias_wd_attr.o: C:/Users/NV-Smart03/Downloads/sdk_zigbee_v3.0/zigbee1.0/zigbee/zcl/security_safety/zcl_ias_wd_attr.c
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -I../../../apps/common -I../../../apps/sampleLight -I../../../platform -I../../../proj/common -I../../../proj -I../../../zigbee/common/includes -I../../../zigbee/zbapi -I../../../zigbee/bdb/includes -I../../../zigbee/gp -I../../../zigbee/zcl -I../../../zigbee/ota -I../../../zigbee/wwah -I../../../zbhci -DROUTER=1 -DMCU_CORE_8258=1 -D__PROJECT_TL_DIMMABLE_LIGHT__=1 -Wall -O2 -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

zigbee/zcl/security_safety/zcl_ias_zone.o: C:/Users/NV-Smart03/Downloads/sdk_zigbee_v3.0/zigbee1.0/zigbee/zcl/security_safety/zcl_ias_zone.c
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -I../../../apps/common -I../../../apps/sampleLight -I../../../platform -I../../../proj/common -I../../../proj -I../../../zigbee/common/includes -I../../../zigbee/zbapi -I../../../zigbee/bdb/includes -I../../../zigbee/gp -I../../../zigbee/zcl -I../../../zigbee/ota -I../../../zigbee/wwah -I../../../zbhci -DROUTER=1 -DMCU_CORE_8258=1 -D__PROJECT_TL_DIMMABLE_LIGHT__=1 -Wall -O2 -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

zigbee/zcl/security_safety/zcl_ias_zone_attr.o: C:/Users/NV-Smart03/Downloads/sdk_zigbee_v3.0/zigbee1.0/zigbee/zcl/security_safety/zcl_ias_zone_attr.c
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -I../../../apps/common -I../../../apps/sampleLight -I../../../platform -I../../../proj/common -I../../../proj -I../../../zigbee/common/includes -I../../../zigbee/zbapi -I../../../zigbee/bdb/includes -I../../../zigbee/gp -I../../../zigbee/zcl -I../../../zigbee/ota -I../../../zigbee/wwah -I../../../zbhci -DROUTER=1 -DMCU_CORE_8258=1 -D__PROJECT_TL_DIMMABLE_LIGHT__=1 -Wall -O2 -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


