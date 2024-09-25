MSPM0_SDK_INSTALL_DIR ?= $(abspath ../../../../../..)

include $(MSPM0_SDK_INSTALL_DIR)/imports.mak

CC = "$(IAR_ARMCOMPILER)/bin/iccarm"
LNK = "$(IAR_ARMCOMPILER)/bin/ilinkarm"

OBJECTS = callbacks_JSON.obj MSP_GUI_GUI_JSON.obj MSP_GUI_GUIComm_JSON.obj ti_msp_dl_config.obj HAL_UART_M0G3507.obj main.obj iar_startup_mspm0g350x_iar.obj
NAME = gc_simple_json

CFLAGS += -I.. \
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

LFLAGS += "$(MSPM0_SDK_INSTALL_DIR)/source/ti/gui_composer/lib/iar/m0p/gui_composer.a" \
    "$(MSPM0_SDK_INSTALL_DIR)/source/ti/driverlib/lib/iar/m0p/mspm0g1x0x_g3x0x/driverlib.a" \
    "-L$(MSPM0_SDK_INSTALL_DIR)/source" \
    -L.. \
    --config ../iar/mspm0g3507.icf \
    --map "$(NAME).map" \
    --silent \
    --semihosting=iar_breakpoint \
    --cpu=Cortex-M0+

all: $(NAME).out

callbacks_JSON.obj: ../callbacks_JSON.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

MSP_GUI_GUI_JSON.obj: ../MSP_GUI/GUI_JSON.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

MSP_GUI_GUIComm_JSON.obj: ../MSP_GUI/GUIComm_JSON.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

ti_msp_dl_config.obj: ../ti_msp_dl_config.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

HAL_UART_M0G3507.obj: ../HAL_UART_M0G3507.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

main.obj: ../main.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

iar_startup_mspm0g350x_iar.obj: ../iar/startup_mspm0g350x_iar.c
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
