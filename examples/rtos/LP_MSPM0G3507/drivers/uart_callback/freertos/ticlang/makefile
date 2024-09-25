MSPM0_SDK_INSTALL_DIR ?= $(abspath ../../../../../../..)

include $(MSPM0_SDK_INSTALL_DIR)/imports.mak

KERNEL_BUILD := $(MSPM0_SDK_INSTALL_DIR)/kernel/freertos/builds/LP_MSPM0G3507/release

CC = "$(TICLANG_ARMCOMPILER)/bin/tiarmclang"
LNK = "$(TICLANG_ARMCOMPILER)/bin/tiarmclang"

OBJECTS = main_freertos.obj ti_drivers_config.obj ti_msp_dl_config.obj uartcallback.obj freertos_ticlang_startup_mspm0g3507_ticlang.obj
NAME = uart_callback

CFLAGS += -I../.. \
    -D__MSPM0G3507__ \
    -O2 \
    "-I$(MSPM0_SDK_INSTALL_DIR)/source/third_party/CMSIS/Core/Include" \
    "-I$(MSPM0_SDK_INSTALL_DIR)/kernel/freertos/Source/include" \
    "-I$(MSPM0_SDK_INSTALL_DIR)/source" \
    "-I$(MSPM0_SDK_INSTALL_DIR)/kernel/freertos/Source/portable/TI_ARM_CLANG/ARM_CM0" \
    "-I$(MSPM0_SDK_INSTALL_DIR)/source/ti/posix/ticlang" \
    "-I$(KERNEL_BUILD)" \
    -gdwarf-3 \
    -mcpu=cortex-m0plus \
    -march=thumbv6m \
    -mfloat-abi=soft \
    -mthumb

LFLAGS += "-l$(MSPM0_SDK_INSTALL_DIR)/source/ti/drivers/lib/ticlang/m0p/drivers_mspm0g1x0x_g3x0x.a" \
    -i${WORKSPACE_LOC}/freertos_builds_LP_MSPM0G3507_release_ticlang/Debug \
    -lfreertos_builds_LP_MSPM0G3507_release_ticlang.lib \
    "-l$(MSPM0_SDK_INSTALL_DIR)/source/ti/driverlib/lib/ticlang/m0p/mspm0g1x0x_g3x0x/driverlib.a" \
    "-L$(MSPM0_SDK_INSTALL_DIR)/source" \
    -L../.. \
    ../../freertos/ticlang/mspm0g3507.cmd \
    "-Wl,-m,$(NAME).map" \
    -Wl,--rom_model \
    -Wl,--warn_sections \
    "-L$(TICLANG_ARMCOMPILER)/lib" \
    -llibc.a

all: $(NAME).out

$(KERNEL_BUILD)/ticlang/freertos.lib:
	@ $(ECHOBLANKLINE)
	@ echo $@ is not built.
	@ echo You can build it by issuing $(MAKE) in $(dir $@).
	@ $(ECHOBLANKLINE)

main_freertos.obj: ../../main_freertos.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

ti_drivers_config.obj: ../../ti_drivers_config.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

ti_msp_dl_config.obj: ../../ti_msp_dl_config.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

uartcallback.obj: ../../uartcallback.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

freertos_ticlang_startup_mspm0g3507_ticlang.obj: ../../freertos/ticlang/startup_mspm0g3507_ticlang.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

$(NAME).out: $(OBJECTS) $(KERNEL_BUILD)/ticlang/freertos.lib
	@ echo linking $@
	@ $(LNK) -Wl,-u,_c_int00 $(OBJECTS)  $(LFLAGS) -o $(NAME).out

clean:
	@ echo Cleaning...
	@ $(RM) $(OBJECTS) > $(DEVNULL) 2>&1
	@ $(RM) $(NAME).out > $(DEVNULL) 2>&1
	@ $(RM) $(NAME).map > $(DEVNULL) 2>&1
