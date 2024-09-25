MSPM0_SDK_INSTALL_DIR ?= $(abspath ../../../../../..)

include $(MSPM0_SDK_INSTALL_DIR)/imports.mak

CC = "$(TICLANG_ARMCOMPILER)/bin/tiarmclang"
LNK = "$(TICLANG_ARMCOMPILER)/bin/tiarmclang"

OBJECTS = MSP_GUI_GUI_mpack.obj MSP_GUI_GUIComm_mpack.obj MSP_GUI_mpack.obj callbacks_mpack.obj HAL_UART_M0L1306.obj cuff.obj main.obj ti_msp_dl_config.obj ticlang_startup_mspm0l130x_ticlang.obj
NAME = blood_pressure_monitor

CFLAGS += -I.. \
    -D__MSPM0L1306__ \
    -O2 \
    "-I$(MSPM0_SDK_INSTALL_DIR)/source/third_party/CMSIS/Core/Include" \
    "-I$(MSPM0_SDK_INSTALL_DIR)/source" \
    -gdwarf-3 \
    -mcpu=cortex-m0plus \
    -march=thumbv6m \
    -mfloat-abi=soft \
    -mthumb

LFLAGS += "-l$(MSPM0_SDK_INSTALL_DIR)/source/ti/gui_composer/lib/ticlang/m0p/gui_composer.a" \
    "-l$(MSPM0_SDK_INSTALL_DIR)/source/ti/driverlib/lib/ticlang/m0p/mspm0l11xx_l13xx/driverlib.a" \
    "-L$(MSPM0_SDK_INSTALL_DIR)/source" \
    -L.. \
    ../ticlang/mspm0l1306.cmd \
    "-Wl,-m,$(NAME).map" \
    -Wl,--rom_model \
    -Wl,--warn_sections \
    "-L$(TICLANG_ARMCOMPILER)/lib" \
    -llibc.a

all: $(NAME).out

MSP_GUI_GUI_mpack.obj: ../MSP_GUI/GUI_mpack.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

MSP_GUI_GUIComm_mpack.obj: ../MSP_GUI/GUIComm_mpack.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

MSP_GUI_mpack.obj: ../MSP_GUI/mpack.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

callbacks_mpack.obj: ../callbacks_mpack.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

HAL_UART_M0L1306.obj: ../HAL_UART_M0L1306.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

cuff.obj: ../cuff.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

main.obj: ../main.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

ti_msp_dl_config.obj: ../ti_msp_dl_config.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

ticlang_startup_mspm0l130x_ticlang.obj: ../ticlang/startup_mspm0l130x_ticlang.c
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
