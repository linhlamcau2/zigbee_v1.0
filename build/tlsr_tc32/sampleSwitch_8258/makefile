################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include zigbee/zdo/subdir.mk
-include zigbee/zcl/zll_commissioning/subdir.mk
-include zigbee/zcl/zcl_wwah/subdir.mk
-include zigbee/zcl/subdir.mk
-include zigbee/zcl/smart_energy/subdir.mk
-include zigbee/zcl/security_safety/subdir.mk
-include zigbee/zcl/ota_upgrading/subdir.mk
-include zigbee/zcl/measument_sensing/subdir.mk
-include zigbee/zcl/light_color_control/subdir.mk
-include zigbee/zcl/hvac/subdir.mk
-include zigbee/zcl/general/subdir.mk
-include zigbee/zcl/commissioning/subdir.mk
-include zigbee/zcl/closures/subdir.mk
-include zigbee/wwah/subdir.mk
-include zigbee/ss/subdir.mk
-include zigbee/ota/subdir.mk
-include zigbee/mac/subdir.mk
-include zigbee/gp/subdir.mk
-include zigbee/common/subdir.mk
-include zigbee/bdb/subdir.mk
-include zigbee/aps/subdir.mk
-include zigbee/af/subdir.mk
-include zbhci/subdir.mk
-include zbhci/usbPrint/subdir.mk
-include zbhci/usbHID/subdir.mk
-include zbhci/usbCDC/subdir.mk
-include zbhci/uart/subdir.mk
-include proj/os/subdir.mk
-include proj/drivers/usb/subdir.mk
-include proj/drivers/usb/app/subdir.mk
-include proj/drivers/subdir.mk
-include proj/common/subdir.mk
-include platform/services/subdir.mk
-include platform/chip_8258/subdir.mk
-include platform/chip_8258/flash/subdir.mk
-include platform/boot/subdir.mk
-include platform/boot/8278/subdir.mk
-include platform/boot/826x/subdir.mk
-include platform/boot/8258/subdir.mk
-include apps/sampleSwitch/subdir.mk
-include apps/common/rd_log/subdir.mk
-include apps/common/rd_in_out/subdir.mk
-include apps/common/subdir.mk
-include objects.mk

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LST += \
sampleSwitch_8258.lst \

FLASH_IMAGE += \

SIZEDUMMY += \
sizedummy \


# All Target
all: pre-build main-build

# Main-build Target
main-build: sampleSwitch_8258.elf secondary-outputs

# Tool invocations
sampleSwitch_8258.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: TC32 C Linker'
	tc32-elf-ld --gc-sections -L../../../zigbee/lib/tc32 -L../../../platform/lib -T ../boot.link -o"sampleSwitch_8258.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

sampleSwitch_8258.lst: sampleSwitch_8258.elf
	@echo 'Invoking: TC32 Create Extended Listing'
	tc32-elf-objdump -x -D -l -S sampleSwitch_8258.elf  >"sampleSwitch_8258.lst"
	@echo 'Finished building: $@'
	@echo ' '

: sampleSwitch_8258.elf
	@echo 'Create Flash image (binary format)'
	tc32-elf-objcopy sampleSwitch_8258.elf
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: sampleSwitch_8258.elf
	@echo 'Invoking: Print Size'
	tc32-elf-size -t sampleSwitch_8258.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(LST)$(SIZEDUMMY) sampleSwitch_8258.elf
	-@echo ' '

pre-build:
	-"../../../tools/tl_link_load.sh" "../../../platform/boot/8258/boot_8258.link" "C:\Users\NV-Smart03\Downloads\sdk_zigbee_v3.0\zigbee1.0\build\tlsr_tc32/boot.link"
	-@echo ' '

post-build:
	-"../../../tools/tl_check_fw.sh" sampleSwitch_8258 tc32
	-@echo ' '

secondary-outputs: $(LST) $(FLASH_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY: main-build pre-build post-build

-include ../makefile.targets
