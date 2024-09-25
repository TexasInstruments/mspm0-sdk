MSPM0_SDK_INSTALL_DIR ?= $(abspath ../../../../../../..)

include $(MSPM0_SDK_INSTALL_DIR)/imports.mak

KERNEL_BUILD := $(MSPM0_SDK_INSTALL_DIR)/kernel/freertos/builds/LP_MSPM0G3507/release

CC = "$(IAR_ARMCOMPILER)/bin/iccarm"
LNK = "$(IAR_ARMCOMPILER)/bin/ilinkarm"

SYSCFG_CMD_STUB = $(SYSCONFIG_TOOL) --compiler iar --product $(MSPM0_SDK_INSTALL_DIR)/.metadata/product.json
SYSCFG_FILES := $(shell $(SYSCFG_CMD_STUB) --listGeneratedFiles --listReferencedFiles --output . ../../blink_led.syscfg)

SYSCFG_C_FILES = $(filter %.c,$(SYSCFG_FILES))
SYSCFG_H_FILES = $(filter %.h,$(SYSCFG_FILES))
SYSCFG_OPT_FILES = $(filter %.opt,$(SYSCFG_FILES))

OBJECTS = main.obj main-blinky.obj freertos_iar_startup_mspm0g350x_iar.obj $(patsubst %.c,%.obj,$(notdir $(SYSCFG_C_FILES)))
NAME = blink_led

CFLAGS += -I../.. \
    -I. \
    $(addprefix -f,$(SYSCFG_OPT_FILES)) \
    -D__MSPM0G3507__ \
    -Om \
    "-I$(MSPM0_SDK_INSTALL_DIR)/source/third_party/CMSIS/Core/Include" \
    "-I$(MSPM0_SDK_INSTALL_DIR)/kernel/freertos/Source/include" \
    "-I$(MSPM0_SDK_INSTALL_DIR)/source" \
    "-I$(MSPM0_SDK_INSTALL_DIR)/kernel/freertos/Source/portable/IAR/ARM_CM0" \
    "-I$(MSPM0_SDK_INSTALL_DIR)/source/ti/posix/iar" \
    "-I$(KERNEL_BUILD)" \
    --debug \
    --silent \
    -e \
    --aeabi \
    --thumb \
    --diag_suppress=Pa050 \
    --cpu=Cortex-M0+ \
    --vla

LFLAGS += "$(MSPM0_SDK_INSTALL_DIR)/source/ti/driverlib/lib/iar/m0p/mspm0g1x0x_g3x0x/driverlib.a" \
    "-L$(MSPM0_SDK_INSTALL_DIR)/source" \
    -L../.. \
    "$(KERNEL_BUILD)/iar/freertos.lib" \
    --config ../../freertos/iar/mspm0g3507.icf \
    --map "$(NAME).map" \
    --silent \
    --semihosting=iar_breakpoint \
    --cpu=Cortex-M0+

all: $(NAME).out

$(KERNEL_BUILD)/iar/freertos.lib:
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
	@ $(CC) $(CFLAGS) $$< -o $$@
endef

$(foreach c_file,$(SYSCFG_C_FILES),$(eval $(call C_RULE,$(c_file))))

main.obj: ../../main.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

main-blinky.obj: ../../main-blinky.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

freertos_iar_startup_mspm0g350x_iar.obj: ../../freertos/iar/startup_mspm0g350x_iar.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

$(NAME).out: $(OBJECTS) $(KERNEL_BUILD)/iar/freertos.lib
	@ echo linking $@
	@ $(LNK)  $(OBJECTS)  $(LFLAGS) -o $(NAME).out

clean:
	@ echo Cleaning...
	@ $(RM) $(OBJECTS) > $(DEVNULL) 2>&1
	@ $(RM) $(NAME).out > $(DEVNULL) 2>&1
	@ $(RM) $(NAME).map > $(DEVNULL) 2>&1
	@ $(RM) $(SYSCFG_FILES)> $(DEVNULL) 2>&1
