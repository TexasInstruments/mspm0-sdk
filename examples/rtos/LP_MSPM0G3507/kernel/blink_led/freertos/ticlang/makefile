MSPM0_SDK_INSTALL_DIR ?= $(abspath ../../../../../../..)

include $(MSPM0_SDK_INSTALL_DIR)/imports.mak

KERNEL_BUILD := $(MSPM0_SDK_INSTALL_DIR)/kernel/freertos/builds/LP_MSPM0G3507/release

CC = "$(TICLANG_ARMCOMPILER)/bin/tiarmclang"
LNK = "$(TICLANG_ARMCOMPILER)/bin/tiarmclang"

SYSCFG_CMD_STUB = $(SYSCONFIG_TOOL) --compiler ticlang --product $(MSPM0_SDK_INSTALL_DIR)/.metadata/product.json
SYSCFG_FILES := $(shell $(SYSCFG_CMD_STUB) --listGeneratedFiles --listReferencedFiles --output . ../../blink_led.syscfg)

SYSCFG_C_FILES = $(filter %.c,$(SYSCFG_FILES))
SYSCFG_H_FILES = $(filter %.h,$(SYSCFG_FILES))
SYSCFG_OPT_FILES = $(filter %.opt,$(SYSCFG_FILES))

OBJECTS = main.obj main-blinky.obj freertos_ticlang_startup_mspm0g350x_ticlang.obj $(patsubst %.c,%.obj,$(notdir $(SYSCFG_C_FILES)))
NAME = blink_led

CFLAGS += -I../.. \
    -I. \
    $(addprefix @,$(SYSCFG_OPT_FILES)) \
    -D__MSPM0G3507__ \
    -O2 \
    "-I$(MSPM0_SDK_INSTALL_DIR)/source/third_party/CMSIS/Core/Include" \
    "-I$(MSPM0_SDK_INSTALL_DIR)/kernel/freertos/Source/include" \
    "-I$(MSPM0_SDK_INSTALL_DIR)/source" \
    "-I$(MSPM0_SDK_INSTALL_DIR)/kernel/freertos/Source/portable/TI_ARM_CLANG/ARM_CM0" \
    "-I$(MSPM0_SDK_INSTALL_DIR)/source/ti/posix/ticlang" \
    "-I$(KERNEL_BUILD)" \
    -gdwarf-3 \
    -mcpu=cortex-m0plus \
    -march=thumbv6m \
    -mfloat-abi=soft \
    -mthumb

LFLAGS += -i${WORKSPACE_LOC}/freertos_builds_LP_MSPM0G3507_release_ticlang/Debug \
    -lfreertos_builds_LP_MSPM0G3507_release_ticlang.lib \
    "-l$(MSPM0_SDK_INSTALL_DIR)/source/ti/driverlib/lib/ticlang/m0p/mspm0g1x0x_g3x0x/driverlib.a" \
    "-L$(MSPM0_SDK_INSTALL_DIR)/source" \
    -L../.. \
    ../../freertos/ticlang/mspm0g3507.cmd \
    "-Wl,-m,$(NAME).map" \
    -Wl,--rom_model \
    -Wl,--warn_sections \
    "-L$(TICLANG_ARMCOMPILER)/lib" \
    -llibc.a

all: $(NAME).out

$(KERNEL_BUILD)/ticlang/freertos.lib:
	@ $(ECHOBLANKLINE)
	@ echo $@ is not built.
	@ echo You can build it by issuing $(MAKE) in $(dir $@).
	@ $(ECHOBLANKLINE)

.INTERMEDIATE: syscfg
$(SYSCFG_FILES): syscfg
	@ echo generation complete

syscfg: ../../blink_led.syscfg
	@ echo Generating configuration files...
	@ $(SYSCFG_CMD_STUB) --output $(@D) $<

define C_RULE
$(basename $(notdir $(1))).obj: $(1) $(SYSCFG_H_FILES)
	@ echo Building $$@
	@ $(CC) $(CFLAGS) -c $$< -o $$@
endef

$(foreach c_file,$(SYSCFG_C_FILES),$(eval $(call C_RULE,$(c_file))))

main.obj: ../../main.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

main-blinky.obj: ../../main-blinky.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

freertos_ticlang_startup_mspm0g350x_ticlang.obj: ../../freertos/ticlang/startup_mspm0g350x_ticlang.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

$(NAME).out: $(OBJECTS) $(KERNEL_BUILD)/ticlang/freertos.lib
	@ echo linking $@
	@ $(LNK) -Wl,-u,_c_int00 $(OBJECTS)  $(LFLAGS) -o $(NAME).out

clean:
	@ echo Cleaning...
	@ $(RM) $(OBJECTS) > $(DEVNULL) 2>&1
	@ $(RM) $(NAME).out > $(DEVNULL) 2>&1
	@ $(RM) $(NAME).map > $(DEVNULL) 2>&1
	@ $(RM) $(SYSCFG_FILES)> $(DEVNULL) 2>&1
