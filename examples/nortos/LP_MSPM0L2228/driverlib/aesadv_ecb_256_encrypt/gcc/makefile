MSPM0_SDK_INSTALL_DIR ?= $(abspath ../../../../../..)

include $(MSPM0_SDK_INSTALL_DIR)/imports.mak

CC = "$(GCC_ARMCOMPILER)/bin/arm-none-eabi-gcc"
LNK = "$(GCC_ARMCOMPILER)/bin/arm-none-eabi-gcc"

OBJECTS = aesadv_ecb_256_encrypt.obj ti_msp_dl_config.obj gcc_startup_mspm0l222x_gcc.obj
NAME = aesadv_ecb_256_encrypt

CFLAGS += -I.. \
    -D__MSPM0L2228__ \
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

LFLAGS += "-L$(MSPM0_SDK_INSTALL_DIR)/source/ti/driverlib/lib/gcc/m0p/mspm0l122x_l222x" \
    -nostartfiles \
    -l:driverlib.a \
    -Wl,-T,../gcc/mspm0l2228.lds \
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

aesadv_ecb_256_encrypt.obj: ../aesadv_ecb_256_encrypt.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

ti_msp_dl_config.obj: ../ti_msp_dl_config.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

gcc_startup_mspm0l222x_gcc.obj: ../gcc/startup_mspm0l222x_gcc.c
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
