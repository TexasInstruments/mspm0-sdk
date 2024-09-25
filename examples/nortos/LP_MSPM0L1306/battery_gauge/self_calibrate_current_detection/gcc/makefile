MSPM0_SDK_INSTALL_DIR ?= $(abspath ../../../../../..)

include $(MSPM0_SDK_INSTALL_DIR)/imports.mak

CC = "$(GCC_ARMCOMPILER)/bin/arm-none-eabi-gcc"
LNK = "$(GCC_ARMCOMPILER)/bin/arm-none-eabi-gcc"

OBJECTS = main.obj App.obj Driver_MSPM0Lxx_BSP.obj Driver_MSPM0Lxx_Driver.obj UserConfig_UserConfig.obj gcc_startup_mspm0l130x_gcc.obj
NAME = self_calibrate_current_detection

CFLAGS += -I.. \
    -D__MSPM0L1306__ \
    -O2 \
    "-I$(MSPM0_SDK_INSTALL_DIR)/source/third_party/CMSIS/Core/Include" \
    "-I$(MSPM0_SDK_INSTALL_DIR)/source" \
    -mcpu=cortex-m0plus \
    -march=armv6-m \
    -mthumb \
    -std=c99 \
    -mfloat-abi=soft \
    -ffunction-sections \
    -fdata-sections \
    -g \
    -gstrict-dwarf \
    -Wall \
    "-I$(GCC_ARMCOMPILER)/arm-none-eabi/include/newlib-nano" \
    "-I$(GCC_ARMCOMPILER)/arm-none-eabi/include"

LFLAGS += "-L$(MSPM0_SDK_INSTALL_DIR)/source/ti/iqmath/lib/gcc/m0p/rts" \
    -l:iqmath.a \
    "-L$(MSPM0_SDK_INSTALL_DIR)/source/ti/driverlib/lib/gcc/m0p/mspm0l11xx_l13xx" \
    -nostartfiles \
    -l:driverlib.a \
    -Wl,-T,../gcc/mspm0l1306.lds \
    "-Wl,-Map,$(NAME).map" \
    "-L$(MSPM0_SDK_INSTALL_DIR)/source" \
    -L.. \
    -march=armv6-m \
    -mthumb \
    -static \
    -Wl,--gc-sections \
    "-L$(GCC_ARMCOMPILER)/arm-none-eabi/lib/thumb/v6-m/nofp" \
    -lgcc \
    -lc \
    -lm \
    -lnosys \
    --specs=nano.specs

all: $(NAME).out

main.obj: ../main.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

App.obj: ../App.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

Driver_MSPM0Lxx_BSP.obj: ../Driver/MSPM0Lxx_BSP.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

Driver_MSPM0Lxx_Driver.obj: ../Driver/MSPM0Lxx_Driver.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

UserConfig_UserConfig.obj: ../UserConfig/UserConfig.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

gcc_startup_mspm0l130x_gcc.obj: ../gcc/startup_mspm0l130x_gcc.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

$(NAME).out: $(OBJECTS)
	@ echo linking $@
	@ $(LNK)  $(OBJECTS)  $(LFLAGS) -o $(NAME).out

clean:
	@ echo Cleaning...
	@ $(RM) $(OBJECTS) > $(DEVNULL) 2>&1
	@ $(RM) $(NAME).out > $(DEVNULL) 2>&1
	@ $(RM) $(NAME).map > $(DEVNULL) 2>&1
