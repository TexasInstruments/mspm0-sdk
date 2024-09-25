MSPM0_SDK_INSTALL_DIR ?= $(abspath ../../../../../..)

include $(MSPM0_SDK_INSTALL_DIR)/imports.mak

CC = "$(TICLANG_ARMCOMPILER)/bin/tiarmclang"
LNK = "$(TICLANG_ARMCOMPILER)/bin/tiarmclang"

OBJECTS = main.obj Gauge_App.obj Driver_MSPM0Lxx_BSP.obj Driver_MSPM0Lxx_Driver.obj Gauge_Algorithm_VoltGauge.obj Gauge_UserConfig_Gauge_UserConfig.obj ticlang_startup_mspm0l130x_ticlang.obj
NAME = gauge_level1

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

LFLAGS += "-l$(MSPM0_SDK_INSTALL_DIR)/source/ti/iqmath/lib/ticlang/m0p/rts/iqmath.a" \
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

main.obj: ../main.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

Gauge_App.obj: ../Gauge_App.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

Driver_MSPM0Lxx_BSP.obj: ../Driver/MSPM0Lxx_BSP.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

Driver_MSPM0Lxx_Driver.obj: ../Driver/MSPM0Lxx_Driver.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

Gauge_Algorithm_VoltGauge.obj: ../Gauge_Algorithm/VoltGauge.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

Gauge_UserConfig_Gauge_UserConfig.obj: ../Gauge_UserConfig/Gauge_UserConfig.c
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
