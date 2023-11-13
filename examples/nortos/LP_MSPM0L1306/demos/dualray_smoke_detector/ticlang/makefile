MSPM0_SDK_INSTALL_DIR ?= $(abspath ../../../../../..)

include $(MSPM0_SDK_INSTALL_DIR)/imports.mak

CC = "$(TICLANG_ARMCOMPILER)/bin/tiarmclang"
LNK = "$(TICLANG_ARMCOMPILER)/bin/tiarmclang"

SYSCFG_CMD_STUB = $(SYSCONFIG_TOOL) --compiler ticlang --product $(MSPM0_SDK_INSTALL_DIR)/.metadata/product.json
SYSCFG_FILES := $(shell $(SYSCFG_CMD_STUB) --listGeneratedFiles --listReferencedFiles --output . ../DualRaySmokeAFE_MSPM0_TPS880x.syscfg)

SYSCFG_C_FILES = $(filter %.c,$(SYSCFG_FILES))
SYSCFG_H_FILES = $(filter %.h,$(SYSCFG_FILES))

OBJECTS = Src_DualRaySmokeAFE_GUI_jsmn_jsmn.obj Src_DualRaySmokeAFE_GUI_DualRaySmokeAFE_GUI.obj Src_DualRaySmokeAFE_HAL_MSPM0x_TPS880x_tps880x_mspm0_i2c_tps880x_mspm0_i2c.obj Src_DualRaySmokeAFE_HAL_MSPM0x_TPS880x_DualRaySmokeAFE_HAL_AFE_MSPM0_TPS880x.obj Src_DualRaySmokeAFE_HAL_MSPM0x_TPS880x_DualRaySmokeAFE_HAL_GUIComm_UART_MSPM0_TPS880x.obj Src_DualRaySmokeAFE_HAL_MSPM0x_TPS880x_DualRaySmokeAFE_HAL_IO_MSPM0_TPS880x.obj Src_DualRaySmokeAFE_HAL_MSPM0x_TPS880x_DualRaySmokeAFE_HAL_Sounder_MSPM0_TPS880x.obj Src_DualRaySmokeAFE_HAL_MSPM0x_TPS880x_DualRaySmokeAFE_HAL_System_MSPM0_TPS880x.obj Src_DualRaySmokeAFE_HAL_MSPM0x_TPS880x_DualRaySmokeAFE_HAL_Timing_MSPM0_TPS880x.obj Src_DualRaySmokeAFELib_DualRaySmokeAFELib_DetectionAlg.obj Src_DualRaySmokeAFELib_DualRaySmokeAFELib_Measurement.obj Src_DualRaySmokeAFE_App_Config.obj Src_DualRaySmokeAFE_App_GuiComm.obj Src_DualRaySmokeAFE_App_Main.obj ticlang_startup_mspm0l1306_ticlang.obj $(patsubst %.c,%.obj,$(notdir $(SYSCFG_C_FILES)))
NAME = dualray_smoke_detector

CFLAGS = -I.. \
    -I. \
    -D__AFE_TPS880X__ \
    -D__ENABLE_GUI__ \
    -D__MSPM0L1306__ \
    -O2 \
    "-I$(MSPM0_SDK_INSTALL_DIR)/source/third_party/CMSIS/Core/Include" \
    "-I$(MSPM0_SDK_INSTALL_DIR)/source" \
    -gdwarf-3 \
    -mcpu=cortex-m0plus \
    -march=thumbv6m \
    -mfloat-abi=soft \
    -mthumb

LFLAGS = "-l$(MSPM0_SDK_INSTALL_DIR)/source/ti/gui_composer/lib/ticlang/m0p/gui_composer.a" \
    "-l$(MSPM0_SDK_INSTALL_DIR)/source/ti/driverlib/lib/ticlang/m0p/mspm0l11xx_l13xx/driverlib.a" \
    ../ticlang/mspm0l1306.cmd \
    "-Wl,-m,$(NAME).map" \
    -Wl,--rom_model \
    -Wl,--warn_sections \
    "-L$(TICLANG_ARMCOMPILER)/lib" \
    -llibc.a
all: $(NAME).out

.INTERMEDIATE: syscfg
$(SYSCFG_FILES): syscfg
	@ echo generation complete

syscfg: ../DualRaySmokeAFE_MSPM0_TPS880x.syscfg
	@ echo Generating configuration files...
	@ $(SYSCFG_CMD_STUB) --output $(@D) $<

define C_RULE
$(basename $(notdir $(1))).obj: $(1) $(SYSCFG_H_FILES)
	@ echo Building $$@
	@ $(CC) $(CFLAGS) -c $$< -o $$@
endef

$(foreach c_file,$(SYSCFG_C_FILES),$(eval $(call C_RULE,$(c_file))))

Src_DualRaySmokeAFE_GUI_jsmn_jsmn.obj: ../Src/DualRaySmokeAFE_GUI/jsmn/jsmn.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

Src_DualRaySmokeAFE_GUI_DualRaySmokeAFE_GUI.obj: ../Src/DualRaySmokeAFE_GUI/DualRaySmokeAFE_GUI.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

Src_DualRaySmokeAFE_HAL_MSPM0x_TPS880x_tps880x_mspm0_i2c_tps880x_mspm0_i2c.obj: ../Src/DualRaySmokeAFE_HAL/MSPM0x_TPS880x/tps880x_mspm0_i2c/tps880x_mspm0_i2c.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

Src_DualRaySmokeAFE_HAL_MSPM0x_TPS880x_DualRaySmokeAFE_HAL_AFE_MSPM0_TPS880x.obj: ../Src/DualRaySmokeAFE_HAL/MSPM0x_TPS880x/DualRaySmokeAFE_HAL_AFE_MSPM0_TPS880x.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

Src_DualRaySmokeAFE_HAL_MSPM0x_TPS880x_DualRaySmokeAFE_HAL_GUIComm_UART_MSPM0_TPS880x.obj: ../Src/DualRaySmokeAFE_HAL/MSPM0x_TPS880x/DualRaySmokeAFE_HAL_GUIComm_UART_MSPM0_TPS880x.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

Src_DualRaySmokeAFE_HAL_MSPM0x_TPS880x_DualRaySmokeAFE_HAL_IO_MSPM0_TPS880x.obj: ../Src/DualRaySmokeAFE_HAL/MSPM0x_TPS880x/DualRaySmokeAFE_HAL_IO_MSPM0_TPS880x.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

Src_DualRaySmokeAFE_HAL_MSPM0x_TPS880x_DualRaySmokeAFE_HAL_Sounder_MSPM0_TPS880x.obj: ../Src/DualRaySmokeAFE_HAL/MSPM0x_TPS880x/DualRaySmokeAFE_HAL_Sounder_MSPM0_TPS880x.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

Src_DualRaySmokeAFE_HAL_MSPM0x_TPS880x_DualRaySmokeAFE_HAL_System_MSPM0_TPS880x.obj: ../Src/DualRaySmokeAFE_HAL/MSPM0x_TPS880x/DualRaySmokeAFE_HAL_System_MSPM0_TPS880x.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

Src_DualRaySmokeAFE_HAL_MSPM0x_TPS880x_DualRaySmokeAFE_HAL_Timing_MSPM0_TPS880x.obj: ../Src/DualRaySmokeAFE_HAL/MSPM0x_TPS880x/DualRaySmokeAFE_HAL_Timing_MSPM0_TPS880x.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

Src_DualRaySmokeAFELib_DualRaySmokeAFELib_DetectionAlg.obj: ../Src/DualRaySmokeAFELib/DualRaySmokeAFELib_DetectionAlg.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

Src_DualRaySmokeAFELib_DualRaySmokeAFELib_Measurement.obj: ../Src/DualRaySmokeAFELib/DualRaySmokeAFELib_Measurement.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

Src_DualRaySmokeAFE_App_Config.obj: ../Src/DualRaySmokeAFE_App_Config.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

Src_DualRaySmokeAFE_App_GuiComm.obj: ../Src/DualRaySmokeAFE_App_GuiComm.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

Src_DualRaySmokeAFE_App_Main.obj: ../Src/DualRaySmokeAFE_App_Main.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

ticlang_startup_mspm0l1306_ticlang.obj: ../ticlang/startup_mspm0l1306_ticlang.c $(SYSCFG_H_FILES)
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
