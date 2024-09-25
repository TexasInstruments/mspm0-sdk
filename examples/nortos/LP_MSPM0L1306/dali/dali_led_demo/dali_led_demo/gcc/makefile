MSPM0_SDK_INSTALL_DIR ?= $(abspath ../../../../../..)

include $(MSPM0_SDK_INSTALL_DIR)/imports.mak

CC = "$(GCC_ARMCOMPILER)/bin/arm-none-eabi-gcc"
LNK = "$(GCC_ARMCOMPILER)/bin/arm-none-eabi-gcc"

SYSCFG_CMD_STUB = $(SYSCONFIG_TOOL) --compiler gcc --product $(MSPM0_SDK_INSTALL_DIR)/.metadata/product.json
SYSCFG_FILES := $(shell $(SYSCFG_CMD_STUB) --listGeneratedFiles --listReferencedFiles --output . ../dali_led_demo.syscfg)

SYSCFG_C_FILES = $(filter %.c,$(SYSCFG_FILES))
SYSCFG_H_FILES = $(filter %.h,$(SYSCFG_FILES))

OBJECTS = dali_target_main.obj ti_eeprom_emulation_type_a_eeprom_emulation_type_a.obj ti_dali_dali_target_comm.obj ti_dali_dali_target_command.obj ti_dali_dali_target_led_command.obj $(patsubst %.c,%.obj,$(notdir $(SYSCFG_C_FILES)))
NAME = dali_led_demo

CFLAGS = -I.. \
    -I. \
    -O2 \
    @device.opt \
    "-I$(MSPM0_SDK_INSTALL_DIR)/source/third_party/CMSIS/Core/Include" \
    "-I$(MSPM0_SDK_INSTALL_DIR)/source" \
    -mcpu=cortex-m0plus \
    -march=armv6-m \
    -mthumb \
    -std=c99 \
    -mfloat-abi=soft \
    -ffunction-sections \
    -fdata-sections \
    -g \
    -gstrict-dwarf \
    -Wall \
    "-I$(GCC_ARMCOMPILER)/arm-none-eabi/include/newlib-nano" \
    "-I$(GCC_ARMCOMPILER)/arm-none-eabi/include"

LFLAGS = -nostartfiles \
    -Tdevice.lds.genlibs \
    -Wl,-T,../gcc/device_linker.lds \
    "-Wl,-Map,$(NAME).map" \
    "-L$(MSPM0_SDK_INSTALL_DIR)/source" \
    -L${PROJECT_ROOT} \
    -march=armv6-m \
    -mthumb \
    -static \
    -Wl,--gc-sections \
    "-L$(GCC_ARMCOMPILER)/arm-none-eabi/lib/thumb/v6-m/nofp" \
    -lgcc \
    -lc \
    -lm \
    -lnosys \
    --specs=nano.specs
all: $(NAME).out

.INTERMEDIATE: syscfg
$(SYSCFG_FILES): syscfg
	@ echo generation complete

syscfg: ../dali_led_demo.syscfg
	@ echo Generating configuration files...
	@ $(SYSCFG_CMD_STUB) --output $(@D) $<

define C_RULE
$(basename $(notdir $(1))).obj: $(1) $(SYSCFG_H_FILES)
	@ echo Building $$@
	@ $(CC) $(CFLAGS) $$< -c -o $$@
endef

$(foreach c_file,$(SYSCFG_C_FILES),$(eval $(call C_RULE,$(c_file))))

dali_target_main.obj: ../dali_target_main.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

ti_eeprom_emulation_type_a_eeprom_emulation_type_a.obj: ../ti/eeprom/emulation_type_a/eeprom_emulation_type_a.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

ti_dali_dali_target_comm.obj: /home/developer/.cache/bazel/_bazel_developer/2ca1f4ebdc59348ffdc31d97a51a98d5/sandbox/processwrapper-sandbox/1183/execroot/__main__/source/ti/dali/dali_target_comm.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

ti_dali_dali_target_command.obj: /home/developer/.cache/bazel/_bazel_developer/2ca1f4ebdc59348ffdc31d97a51a98d5/sandbox/processwrapper-sandbox/1183/execroot/__main__/source/ti/dali/dali_target_command.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

ti_dali_dali_target_led_command.obj: /home/developer/.cache/bazel/_bazel_developer/2ca1f4ebdc59348ffdc31d97a51a98d5/sandbox/processwrapper-sandbox/1183/execroot/__main__/source/ti/dali/dali_target_led_command.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

$(NAME).out: $(OBJECTS)
	@ echo linking $@
	@ $(LNK)  $(OBJECTS)  $(LFLAGS) -o $(NAME).out

clean:
	@ echo Cleaning...
	@ $(RM) $(OBJECTS) > $(DEVNULL) 2>&1
	@ $(RM) $(NAME).out > $(DEVNULL) 2>&1
	@ $(RM) $(NAME).map > $(DEVNULL) 2>&1
	@ $(RM) $(SYSCFG_FILES)> $(DEVNULL) 2>&1
