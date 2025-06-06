MSPM0_SDK_INSTALL_DIR ?= $(abspath ../../../../../..)

include $(MSPM0_SDK_INSTALL_DIR)/imports.mak

CC = "$(TICLANG_ARMCOMPILER)/bin/tiarmclang"
LNK = "$(TICLANG_ARMCOMPILER)/bin/tiarmclang"

SYSCONFIG_GUI_TOOL = $(dir $(SYSCONFIG_TOOL))sysconfig_gui$(suffix $(SYSCONFIG_TOOL))
SYSCFG_CMD_STUB = $(SYSCONFIG_TOOL) --compiler ticlang --product $(MSPM0_SDK_INSTALL_DIR)/.metadata/product.json
SYSCFG_GUI_CMD_STUB = $(SYSCONFIG_GUI_TOOL) --compiler ticlang --product $(MSPM0_SDK_INSTALL_DIR)/.metadata/product.json
SYSCFG_FILES := $(shell $(SYSCFG_CMD_STUB) --listGeneratedFiles --listReferencedFiles --output . ../LP_MSPM0G1507/Hall_Trap.syscfg)

SYSCFG_C_FILES = $(filter %.c,$(SYSCFG_FILES))
SYSCFG_H_FILES = $(filter %.h,$(SYSCFG_FILES))
SYSCFG_OPT_FILES = $(filter %.opt,$(SYSCFG_FILES))

OBJECTS = main.obj ti_motor_control_bldc_sensored_trap_hall_modules_hal_MSPM0G1507_hal.obj ti_motor_control_bldc_sensored_trap_hall_modules_hall_trap_hall_trap.obj ti_motor_control_bldc_sensored_trap_hall_modules_motor_driver_drv8300_drv8300.obj ti_motor_control_bldc_sensored_trap_hall_modules_motor_driver_drv8311_drv8311.obj ti_motor_control_bldc_sensored_trap_hall_modules_motor_driver_drv8316_drv8316.obj ti_motor_control_bldc_sensored_trap_hall_modules_motor_driver_drv8317_drv8317.obj ti_motor_control_bldc_sensored_trap_hall_modules_motor_driver_drv8323_drv8323.obj ti_motor_control_bldc_sensored_trap_hall_modules_motor_driver_drv8328_drv8328.obj ti_motor_control_bldc_sensored_trap_hall_modules_motor_driver_drv8329_drv8329.obj ti_motor_control_bldc_sensored_trap_hall_modules_motor_driver_drv8353_drv8353.obj tida-010251.obj ticlang_startup_mspm0g350x_ticlang.obj $(patsubst %.c,%.obj,$(notdir $(SYSCFG_C_FILES)))
NAME = tida-010251-gui-firmware

CFLAGS += -I.. \
    -I. \
    $(addprefix @,$(SYSCFG_OPT_FILES)) \
    -D__MSPM0G3507__ \
    "-I$(MSPM0_SDK_INSTALL_DIR)/source/third_party/CMSIS/Core/Include" \
    "-I$(MSPM0_SDK_INSTALL_DIR)/source/ti/motor_control_bldc_sensored_trap_hall/modules/hal/MSPM0G1507" \
    "-I$(MSPM0_SDK_INSTALL_DIR)/source/ti/motor_control_bldc_sensored_trap_hall/modules/hall_trap" \
    "-I$(MSPM0_SDK_INSTALL_DIR)/source/ti/motor_control_bldc_sensored_trap_hall/modules/motor_driver/drv8328" \
    "-I$(MSPM0_SDK_INSTALL_DIR)/source/ti/motor_control_bldc_sensored_trap_hall" \
    "-I$(MSPM0_SDK_INSTALL_DIR)/source" \
    -gdwarf-3 \
    -mcpu=cortex-m0plus \
    -march=thumbv6m \
    -mfloat-abi=soft \
    -mthumb

LFLAGS += "-l$(MSPM0_SDK_INSTALL_DIR)/source/ti/iqmath/lib/ticlang/m0p/mathacl/mspm0g1x0x_g3x0x/iqmath.a" \
    "-l$(MSPM0_SDK_INSTALL_DIR)/source/ti/driverlib/lib/ticlang/m0p/mspm0g1x0x_g3x0x/driverlib.a" \
    "-L$(MSPM0_SDK_INSTALL_DIR)/source" \
    -L.. \
    ../ticlang/mspm0g3507.cmd \
    "-Wl,-m,$(NAME).map" \
    -Wl,--rom_model \
    -Wl,--warn_sections \
    "-L$(TICLANG_ARMCOMPILER)/lib" \
    -llibc.a

all: $(NAME).out

.INTERMEDIATE: syscfg
$(SYSCFG_FILES): syscfg
	@ echo generation complete

syscfg: ../LP_MSPM0G1507/Hall_Trap.syscfg
	@ echo Generating configuration files...
	@ $(SYSCFG_CMD_STUB) --output $(@D) $<


# Helpful hint that the user needs to use a standalone SysConfig installation
$(SYSCONFIG_GUI_TOOL):
	$(error $(dir $(SYSCONFIG_TOOL)) does not contain the GUI framework \
        necessary to launch the SysConfig GUI.  Please set SYSCONFIG_TOOL \
        (in your SDK's imports.mak) to a standalone SysConfig installation \
        rather than one inside CCS)

syscfg-gui: ../LP_MSPM0G1507/Hall_Trap.syscfg $(SYSCONFIG_GUI_TOOL)
	@ echo Opening SysConfig GUI
	@ $(SYSCFG_GUI_CMD_STUB) $<

define C_RULE
$(basename $(notdir $(1))).obj: $(1) $(SYSCFG_H_FILES)
	@ echo Building $$@
	@ $(CC) $(CFLAGS) -c $$< -o $$@
endef

$(foreach c_file,$(SYSCFG_C_FILES),$(eval $(call C_RULE,$(c_file))))

main.obj: ../main.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

ti_motor_control_bldc_sensored_trap_hall_modules_hal_MSPM0G1507_hal.obj: /home/developer/.cache/bazel/_bazel_developer/2ca1f4ebdc59348ffdc31d97a51a98d5/sandbox/processwrapper-sandbox/6305/execroot/__main__/source/ti/motor_control_bldc_sensored_trap_hall/modules/hal/MSPM0G1507/hal.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

ti_motor_control_bldc_sensored_trap_hall_modules_hall_trap_hall_trap.obj: /home/developer/.cache/bazel/_bazel_developer/2ca1f4ebdc59348ffdc31d97a51a98d5/sandbox/processwrapper-sandbox/6305/execroot/__main__/source/ti/motor_control_bldc_sensored_trap_hall/modules/hall_trap/hall_trap.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

ti_motor_control_bldc_sensored_trap_hall_modules_motor_driver_drv8300_drv8300.obj: /home/developer/.cache/bazel/_bazel_developer/2ca1f4ebdc59348ffdc31d97a51a98d5/sandbox/processwrapper-sandbox/6305/execroot/__main__/source/ti/motor_control_bldc_sensored_trap_hall/modules/motor_driver/drv8300/drv8300.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

ti_motor_control_bldc_sensored_trap_hall_modules_motor_driver_drv8311_drv8311.obj: /home/developer/.cache/bazel/_bazel_developer/2ca1f4ebdc59348ffdc31d97a51a98d5/sandbox/processwrapper-sandbox/6305/execroot/__main__/source/ti/motor_control_bldc_sensored_trap_hall/modules/motor_driver/drv8311/drv8311.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

ti_motor_control_bldc_sensored_trap_hall_modules_motor_driver_drv8316_drv8316.obj: /home/developer/.cache/bazel/_bazel_developer/2ca1f4ebdc59348ffdc31d97a51a98d5/sandbox/processwrapper-sandbox/6305/execroot/__main__/source/ti/motor_control_bldc_sensored_trap_hall/modules/motor_driver/drv8316/drv8316.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

ti_motor_control_bldc_sensored_trap_hall_modules_motor_driver_drv8317_drv8317.obj: /home/developer/.cache/bazel/_bazel_developer/2ca1f4ebdc59348ffdc31d97a51a98d5/sandbox/processwrapper-sandbox/6305/execroot/__main__/source/ti/motor_control_bldc_sensored_trap_hall/modules/motor_driver/drv8317/drv8317.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

ti_motor_control_bldc_sensored_trap_hall_modules_motor_driver_drv8323_drv8323.obj: /home/developer/.cache/bazel/_bazel_developer/2ca1f4ebdc59348ffdc31d97a51a98d5/sandbox/processwrapper-sandbox/6305/execroot/__main__/source/ti/motor_control_bldc_sensored_trap_hall/modules/motor_driver/drv8323/drv8323.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

ti_motor_control_bldc_sensored_trap_hall_modules_motor_driver_drv8328_drv8328.obj: /home/developer/.cache/bazel/_bazel_developer/2ca1f4ebdc59348ffdc31d97a51a98d5/sandbox/processwrapper-sandbox/6305/execroot/__main__/source/ti/motor_control_bldc_sensored_trap_hall/modules/motor_driver/drv8328/drv8328.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

ti_motor_control_bldc_sensored_trap_hall_modules_motor_driver_drv8329_drv8329.obj: /home/developer/.cache/bazel/_bazel_developer/2ca1f4ebdc59348ffdc31d97a51a98d5/sandbox/processwrapper-sandbox/6305/execroot/__main__/source/ti/motor_control_bldc_sensored_trap_hall/modules/motor_driver/drv8329/drv8329.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

ti_motor_control_bldc_sensored_trap_hall_modules_motor_driver_drv8353_drv8353.obj: /home/developer/.cache/bazel/_bazel_developer/2ca1f4ebdc59348ffdc31d97a51a98d5/sandbox/processwrapper-sandbox/6305/execroot/__main__/source/ti/motor_control_bldc_sensored_trap_hall/modules/motor_driver/drv8353/drv8353.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

tida-010251.obj: ../tida-010251.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

ticlang_startup_mspm0g350x_ticlang.obj: ../ticlang/startup_mspm0g350x_ticlang.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

$(NAME).out: $(OBJECTS)
	@ echo linking $@
	@ $(LNK) -Wl,-u,_c_int00 $(OBJECTS)  $(LFLAGS) -o $(NAME).out

clean:
	@ echo Cleaning...
	@ $(RM) $(OBJECTS) > $(DEVNULL) 2>&1
	@ $(RM) $(NAME).out > $(DEVNULL) 2>&1
	@ $(RM) $(NAME).map > $(DEVNULL) 2>&1
	@ $(RM) $(SYSCFG_FILES)> $(DEVNULL) 2>&1
