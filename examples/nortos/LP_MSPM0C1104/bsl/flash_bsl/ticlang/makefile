MSPM0_SDK_INSTALL_DIR ?= $(abspath ../../../../../..)

include $(MSPM0_SDK_INSTALL_DIR)/imports.mak

CC = "$(TICLANG_ARMCOMPILER)/bin/tiarmclang"
LNK = "$(TICLANG_ARMCOMPILER)/bin/tiarmclang"

OBJECTS = boot_config.obj flashBSL_ci.obj flashBSL_i2c.obj flashBSL_invocation.obj flashBSL_uart.obj flashBSL.obj ticlang_startup_mspm0c1104_ticlang.obj
NAME = flash_bsl

CFLAGS += -I.. \
    -D__MSPM0C1104__ \
    -Oz \
    -mlittle-endian \
    "-I$(MSPM0_SDK_INSTALL_DIR)/source/third_party/CMSIS/Core/Include" \
    "-I$(MSPM0_SDK_INSTALL_DIR)/source" \
    -gdwarf-3 \
    -mcpu=cortex-m0plus \
    -march=thumbv6m \
    -mfloat-abi=soft \
    -mthumb

LFLAGS += -Wl,--stack_size=0 \
    "-l$(MSPM0_SDK_INSTALL_DIR)/source/ti/driverlib/lib/ticlang/m0p/mspm0c110x/driverlib.a" \
    "-L$(MSPM0_SDK_INSTALL_DIR)/source" \
    -L.. \
    ../ticlang/mspm0c1104.cmd \
    "-Wl,-m,$(NAME).map" \
    -Wl,--rom_model \
    -Wl,--warn_sections \
    "-L$(TICLANG_ARMCOMPILER)/lib" \
    -llibc.a

all: $(NAME).out

boot_config.obj: ../boot_config.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

flashBSL_ci.obj: ../flashBSL_ci.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

flashBSL_i2c.obj: ../flashBSL_i2c.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

flashBSL_invocation.obj: ../flashBSL_invocation.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

flashBSL_uart.obj: ../flashBSL_uart.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

flashBSL.obj: ../flashBSL.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

ticlang_startup_mspm0c1104_ticlang.obj: ../ticlang/startup_mspm0c1104_ticlang.c
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
