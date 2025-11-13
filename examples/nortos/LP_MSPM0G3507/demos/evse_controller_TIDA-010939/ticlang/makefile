MSPM0_SDK_INSTALL_DIR ?= $(abspath ../../../../../..)

include $(MSPM0_SDK_INSTALL_DIR)/imports.mak

CC = "$(TICLANG_ARMCOMPILER)/bin/tiarmclang"
LNK = "$(TICLANG_ARMCOMPILER)/bin/tiarmclang"

SYSCONFIG_GUI_TOOL = $(dir $(SYSCONFIG_TOOL))sysconfig_gui$(suffix $(SYSCONFIG_TOOL))
SYSCFG_CMD_STUB = $(SYSCONFIG_TOOL) --compiler ticlang --product $(MSPM0_SDK_INSTALL_DIR)/.metadata/product.json
SYSCFG_GUI_CMD_STUB = $(SYSCONFIG_GUI_TOOL) --compiler ticlang --product $(MSPM0_SDK_INSTALL_DIR)/.metadata/product.json
SYSCFG_FILES := $(shell $(SYSCFG_CMD_STUB) --listGeneratedFiles --listReferencedFiles --output . ../TIDA-010939.syscfg)

SYSCFG_C_FILES = $(filter %.c,$(SYSCFG_FILES))
SYSCFG_H_FILES = $(filter %.h,$(SYSCFG_FILES))
SYSCFG_OPT_FILES = $(filter %.opt,$(SYSCFG_FILES))

OBJECTS = modules_Adc.obj modules_ControlPilot.obj modules_ControlPilot_HAL.obj modules_FirmwareUpdater.obj modules_Gpio.obj modules_MgmtLink.obj modules_PlugLock.obj modules_PowerSwitch.obj modules_Rcd.obj modules_RemoteControlRX.obj modules_RemoteControlTX.obj pb_nanopb_pb_common.obj pb_nanopb_pb_decode.obj pb_nanopb_pb_encode.obj pb_protobuf_tida010939.pb.obj main.obj $(patsubst %.c,%.obj,$(notdir $(SYSCFG_C_FILES)))
NAME = evse_controller_TIDA-010939

CFLAGS += -I.. \
    -I. \
    $(addprefix @,$(SYSCFG_OPT_FILES)) \
    -O2 \
    @device.opt \
    "-I$(MSPM0_SDK_INSTALL_DIR)/source/third_party/CMSIS/Core/Include" \
    -Iexamples/demos/evse_controller_TIDA-010939/includes \
    -Iexamples/demos/evse_controller_TIDA-010939/modules \
    -Iexamples/demos/evse_controller_TIDA-010939/pb/nanopb \
    -Iexamples/demos/evse_controller_TIDA-010939/pb/protobuf \
    "-I$(MSPM0_SDK_INSTALL_DIR)/source" \
    -gdwarf-3 \
    -mcpu=cortex-m0plus \
    -march=thumbv6m \
    -mfloat-abi=soft \
    -mthumb

CPPFLAGS += -I.. \
    -I. \
    $(addprefix @,$(SYSCFG_OPT_FILES)) \
    -O2 \
    @device.opt \
    "-I$(MSPM0_SDK_INSTALL_DIR)/source/third_party/CMSIS/Core/Include" \
    -Iexamples/demos/evse_controller_TIDA-010939/includes \
    -Iexamples/demos/evse_controller_TIDA-010939/modules \
    -Iexamples/demos/evse_controller_TIDA-010939/pb/nanopb \
    -Iexamples/demos/evse_controller_TIDA-010939/pb/protobuf \
    "-I$(MSPM0_SDK_INSTALL_DIR)/source" \
    -gdwarf-3 \
    -mcpu=cortex-m0plus \
    -march=thumbv6m \
    -mfloat-abi=soft \
    -mthumb

LFLAGS += -ldevice.cmd.genlibs \
    "-L$(MSPM0_SDK_INSTALL_DIR)/source" \
    -L.. \
    ../ticlang/device_linker.cmd \
    "-Wl,-m,$(NAME).map" \
    -Wl,--rom_model \
    -Wl,--warn_sections \
    "-L$(TICLANG_ARMCOMPILER)/lib" \
    -llibc.a

all: $(NAME).out

.INTERMEDIATE: syscfg
$(SYSCFG_FILES): syscfg
	@ echo generation complete

syscfg: ../TIDA-010939.syscfg
	@ echo Generating configuration files...
	@ $(SYSCFG_CMD_STUB) --output $(@D) $<


# Helpful hint that the user needs to use a standalone SysConfig installation
$(SYSCONFIG_GUI_TOOL):
	$(error $(dir $(SYSCONFIG_TOOL)) does not contain the GUI framework \
        necessary to launch the SysConfig GUI.  Please set SYSCONFIG_TOOL \
        (in your SDK's imports.mak) to a standalone SysConfig installation \
        rather than one inside CCS)

syscfg-gui: ../TIDA-010939.syscfg $(SYSCONFIG_GUI_TOOL)
	@ echo Opening SysConfig GUI
	@ $(SYSCFG_GUI_CMD_STUB) $<

define C_RULE
$(basename $(notdir $(1))).obj: $(1) $(SYSCFG_H_FILES)
	@ echo Building $$@
	@ $(CC) $(CFLAGS) -c $$< -o $$@
endef

$(foreach c_file,$(SYSCFG_C_FILES),$(eval $(call C_RULE,$(c_file))))

pb_nanopb_pb_common.obj: ../pb/nanopb/pb_common.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

pb_nanopb_pb_decode.obj: ../pb/nanopb/pb_decode.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

pb_nanopb_pb_encode.obj: ../pb/nanopb/pb_encode.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

modules_Adc.obj: ../modules/Adc.cpp $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CPPFLAGS) -c $< -o $@

modules_ControlPilot.obj: ../modules/ControlPilot.cpp $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CPPFLAGS) -c $< -o $@

modules_ControlPilot_HAL.obj: ../modules/ControlPilot_HAL.cpp $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CPPFLAGS) -c $< -o $@

modules_FirmwareUpdater.obj: ../modules/FirmwareUpdater.cpp $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CPPFLAGS) -c $< -o $@

modules_Gpio.obj: ../modules/Gpio.cpp $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CPPFLAGS) -c $< -o $@

modules_MgmtLink.obj: ../modules/MgmtLink.cpp $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CPPFLAGS) -c $< -o $@

modules_PlugLock.obj: ../modules/PlugLock.cpp $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CPPFLAGS) -c $< -o $@

modules_PowerSwitch.obj: ../modules/PowerSwitch.cpp $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CPPFLAGS) -c $< -o $@

modules_Rcd.obj: ../modules/Rcd.cpp $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CPPFLAGS) -c $< -o $@

modules_RemoteControlRX.obj: ../modules/RemoteControlRX.cpp $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CPPFLAGS) -c $< -o $@

modules_RemoteControlTX.obj: ../modules/RemoteControlTX.cpp $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CPPFLAGS) -c $< -o $@

main.obj: ../main.cpp $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CPPFLAGS) -c $< -o $@

$(NAME).out: $(OBJECTS)
	@ echo linking $@
	@ $(LNK) -Wl,-u,_c_int00 $(OBJECTS)  $(LFLAGS) -o $(NAME).out

clean:
	@ echo Cleaning...
	@ $(RM) $(OBJECTS) > $(DEVNULL) 2>&1
	@ $(RM) $(NAME).out > $(DEVNULL) 2>&1
	@ $(RM) $(NAME).map > $(DEVNULL) 2>&1
	@ $(RM) $(SYSCFG_FILES)> $(DEVNULL) 2>&1
