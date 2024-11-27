MSPM0_SDK_INSTALL_DIR ?= $(abspath ../../../../../..)

include $(MSPM0_SDK_INSTALL_DIR)/imports.mak

CC = "$(IAR_ARMCOMPILER)/bin/iccarm"
LNK = "$(IAR_ARMCOMPILER)/bin/ilinkarm"

SYSCONFIG_GUI_TOOL = $(dir $(SYSCONFIG_TOOL))sysconfig_gui$(suffix $(SYSCONFIG_TOOL))
SYSCFG_CMD_STUB = $(SYSCONFIG_TOOL) --compiler iar --product $(MSPM0_SDK_INSTALL_DIR)/.metadata/product.json
SYSCFG_GUI_CMD_STUB = $(SYSCONFIG_GUI_TOOL) --compiler iar --product $(MSPM0_SDK_INSTALL_DIR)/.metadata/product.json
SYSCFG_FILES := $(shell $(SYSCFG_CMD_STUB) --listGeneratedFiles --listReferencedFiles --output . ../bq769x2_TIDA010247.syscfg)

SYSCFG_C_FILES = $(filter %.c,$(SYSCFG_FILES))
SYSCFG_H_FILES = $(filter %.h,$(SYSCFG_FILES))
SYSCFG_OPT_FILES = $(filter %.opt,$(SYSCFG_FILES))

OBJECTS = BQ769x2_Configs_BQ769x2_protocol.obj Communications_CAN_communication.obj Communications_I2C_communication.obj Communications_UART_communication.obj App_task.obj main.obj iar_startup_mspm0g351x_iar.obj $(patsubst %.c,%.obj,$(notdir $(SYSCFG_C_FILES)))
NAME = bq769x2_TIDA010247

CFLAGS += -I.. \
    -I. \
    $(addprefix -f,$(SYSCFG_OPT_FILES)) \
    -D__MSPM0G3519__ \
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

LFLAGS += "$(MSPM0_SDK_INSTALL_DIR)/source/ti/driverlib/lib/iar/m0p/mspm0gx51x/driverlib.a" \
    "-L$(MSPM0_SDK_INSTALL_DIR)/source" \
    -L.. \
    --config ../iar/mspm0g3519.icf \
    --map "$(NAME).map" \
    --silent \
    --semihosting=iar_breakpoint \
    --cpu=Cortex-M0+

all: $(NAME).out

.INTERMEDIATE: syscfg
$(SYSCFG_FILES): syscfg
	@ echo generation complete

syscfg: ../bq769x2_TIDA010247.syscfg
	@ echo Generating configuration files...
	@ $(SYSCFG_CMD_STUB) --output $(@D) $<


# Helpful hint that the user needs to use a standalone SysConfig installation
$(SYSCONFIG_GUI_TOOL):
	$(error $(dir $(SYSCONFIG_TOOL)) does not contain the GUI framework \
        necessary to launch the SysConfig GUI.  Please set SYSCONFIG_TOOL \
        (in your SDK's imports.mak) to a standalone SysConfig installation \
        rather than one inside CCS)

syscfg-gui: ../bq769x2_TIDA010247.syscfg $(SYSCONFIG_GUI_TOOL)
	@ echo Opening SysConfig GUI
	@ $(SYSCFG_GUI_CMD_STUB) $<

define C_RULE
$(basename $(notdir $(1))).obj: $(1) $(SYSCFG_H_FILES)
	@ echo Building $$@
	@ $(CC) $(CFLAGS) $$< -o $$@
endef

$(foreach c_file,$(SYSCFG_C_FILES),$(eval $(call C_RULE,$(c_file))))

BQ769x2_Configs_BQ769x2_protocol.obj: ../BQ769x2_Configs/BQ769x2_protocol.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

Communications_CAN_communication.obj: ../Communications/CAN_communication.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

Communications_I2C_communication.obj: ../Communications/I2C_communication.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

Communications_UART_communication.obj: ../Communications/UART_communication.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

App_task.obj: ../App_task.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

main.obj: ../main.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

iar_startup_mspm0g351x_iar.obj: ../iar/startup_mspm0g351x_iar.c $(SYSCFG_H_FILES)
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
