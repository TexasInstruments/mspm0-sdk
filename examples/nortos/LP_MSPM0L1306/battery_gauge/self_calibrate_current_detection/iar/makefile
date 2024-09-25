MSPM0_SDK_INSTALL_DIR ?= $(abspath ../../../../../..)

include $(MSPM0_SDK_INSTALL_DIR)/imports.mak

CC = "$(IAR_ARMCOMPILER)/bin/iccarm"
LNK = "$(IAR_ARMCOMPILER)/bin/ilinkarm"

OBJECTS = main.obj App.obj Driver_MSPM0Lxx_BSP.obj Driver_MSPM0Lxx_Driver.obj UserConfig_UserConfig.obj iar_startup_mspm0l130x_iar.obj
NAME = self_calibrate_current_detection

CFLAGS += -I.. \
    -D__MSPM0L1306__ \
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

LFLAGS += "$(MSPM0_SDK_INSTALL_DIR)/source/ti/iqmath/lib/iar/m0p/rts/iqmath.a" \
    "$(MSPM0_SDK_INSTALL_DIR)/source/ti/driverlib/lib/iar/m0p/mspm0l11xx_l13xx/driverlib.a" \
    "-L$(MSPM0_SDK_INSTALL_DIR)/source" \
    -L.. \
    --config ../iar/mspm0l1306.icf \
    --map "$(NAME).map" \
    --silent \
    --semihosting=iar_breakpoint \
    --cpu=Cortex-M0+

all: $(NAME).out

main.obj: ../main.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

App.obj: ../App.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

Driver_MSPM0Lxx_BSP.obj: ../Driver/MSPM0Lxx_BSP.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

Driver_MSPM0Lxx_Driver.obj: ../Driver/MSPM0Lxx_Driver.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

UserConfig_UserConfig.obj: ../UserConfig/UserConfig.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

iar_startup_mspm0l130x_iar.obj: ../iar/startup_mspm0l130x_iar.c
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
