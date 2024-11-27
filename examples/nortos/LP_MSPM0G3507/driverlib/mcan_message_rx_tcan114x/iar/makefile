MSPM0_SDK_INSTALL_DIR ?= $(abspath ../../../../../..)

include $(MSPM0_SDK_INSTALL_DIR)/imports.mak

CC = "$(IAR_ARMCOMPILER)/bin/iccarm"
LNK = "$(IAR_ARMCOMPILER)/bin/ilinkarm"

SYSCONFIG_GUI_TOOL = $(dir $(SYSCONFIG_TOOL))sysconfig_gui$(suffix $(SYSCONFIG_TOOL))
SYSCFG_CMD_STUB = $(SYSCONFIG_TOOL) --compiler iar --product $(MSPM0_SDK_INSTALL_DIR)/.metadata/product.json
SYSCFG_GUI_CMD_STUB = $(SYSCONFIG_GUI_TOOL) --compiler iar --product $(MSPM0_SDK_INSTALL_DIR)/.metadata/product.json
SYSCFG_FILES := $(shell $(SYSCFG_CMD_STUB) --listGeneratedFiles --listReferencedFiles --output . ../mcan_message_rx_tcan114x.syscfg)

SYSCFG_C_FILES = $(filter %.c,$(SYSCFG_FILES))
SYSCFG_H_FILES = $(filter %.h,$(SYSCFG_FILES))
SYSCFG_OPT_FILES = $(filter %.opt,$(SYSCFG_FILES))

OBJECTS = mcan_message_rx_tcan114x.obj ti_boosterpacks_tcan114x_tcan114x.obj ti_boosterpacks_tcan114x_tcan114x_spi.obj ti_boosterpacks_tcan114x_tcan114x_wdt.obj iar_startup_mspm0g350x_iar.obj $(patsubst %.c,%.obj,$(notdir $(SYSCFG_C_FILES)))
NAME = mcan_message_rx_tcan114x

CFLAGS += -I.. \
    -I. \
    $(addprefix -f,$(SYSCFG_OPT_FILES)) \
    -D__MSPM0G3507__ \
    -Om \
    "-I$(MSPM0_SDK_INSTALL_DIR)/source/third_party/CMSIS/Core/Include" \
    "-I$(MSPM0_SDK_INSTALL_DIR)/source" \
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
    -L.. \
    --config ../iar/mspm0g3507.icf \
    --map "$(NAME).map" \
    --silent \
    --semihosting=iar_breakpoint \
    --cpu=Cortex-M0+

all: $(NAME).out

.INTERMEDIATE: syscfg
$(SYSCFG_FILES): syscfg
	@ echo generation complete

syscfg: ../mcan_message_rx_tcan114x.syscfg
	@ echo Generating configuration files...
	@ $(SYSCFG_CMD_STUB) --output $(@D) $<


# Helpful hint that the user needs to use a standalone SysConfig installation
$(SYSCONFIG_GUI_TOOL):
	$(error $(dir $(SYSCONFIG_TOOL)) does not contain the GUI framework \
        necessary to launch the SysConfig GUI.  Please set SYSCONFIG_TOOL \
        (in your SDK's imports.mak) to a standalone SysConfig installation \
        rather than one inside CCS)

syscfg-gui: ../mcan_message_rx_tcan114x.syscfg $(SYSCONFIG_GUI_TOOL)
	@ echo Opening SysConfig GUI
	@ $(SYSCFG_GUI_CMD_STUB) $<

define C_RULE
$(basename $(notdir $(1))).obj: $(1) $(SYSCFG_H_FILES)
	@ echo Building $$@
	@ $(CC) $(CFLAGS) $$< -o $$@
endef

$(foreach c_file,$(SYSCFG_C_FILES),$(eval $(call C_RULE,$(c_file))))

mcan_message_rx_tcan114x.obj: ../mcan_message_rx_tcan114x.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

ti_boosterpacks_tcan114x_tcan114x.obj: ../ti/boosterpacks/tcan114x/tcan114x.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

ti_boosterpacks_tcan114x_tcan114x_spi.obj: ../ti/boosterpacks/tcan114x/tcan114x_spi.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

ti_boosterpacks_tcan114x_tcan114x_wdt.obj: ../ti/boosterpacks/tcan114x/tcan114x_wdt.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

iar_startup_mspm0g350x_iar.obj: ../iar/startup_mspm0g350x_iar.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

$(NAME).out: $(OBJECTS)
	@ echo linking $@
	@ $(LNK)  $(OBJECTS)  $(LFLAGS) -o $(NAME).out

clean:
	@ echo Cleaning...
	@ $(RM) $(OBJECTS) > $(DEVNULL) 2>&1
	@ $(RM) $(NAME).out > $(DEVNULL) 2>&1
	@ $(RM) $(NAME).map > $(DEVNULL) 2>&1
	@ $(RM) $(SYSCFG_FILES)> $(DEVNULL) 2>&1
