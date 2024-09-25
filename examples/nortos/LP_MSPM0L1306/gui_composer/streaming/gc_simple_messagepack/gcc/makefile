MSPM0_SDK_INSTALL_DIR ?= $(abspath ../../../../../..)

include $(MSPM0_SDK_INSTALL_DIR)/imports.mak

CC = "$(GCC_ARMCOMPILER)/bin/arm-none-eabi-gcc"
LNK = "$(GCC_ARMCOMPILER)/bin/arm-none-eabi-gcc"

SYSCFG_CMD_STUB = $(SYSCONFIG_TOOL) --compiler gcc --product $(MSPM0_SDK_INSTALL_DIR)/.metadata/product.json
SYSCFG_FILES := $(shell $(SYSCFG_CMD_STUB) --listGeneratedFiles --listReferencedFiles --output . ../gc_simple_messagepack.syscfg)

SYSCFG_C_FILES = $(filter %.c,$(SYSCFG_FILES))
SYSCFG_H_FILES = $(filter %.h,$(SYSCFG_FILES))
SYSCFG_OPT_FILES = $(filter %.opt,$(SYSCFG_FILES))

OBJECTS = callbacks_mpack.obj MSP_GUI_GUI_mpack.obj MSP_GUI_GUIComm_mpack.obj HAL_UART_M0L1306.obj main.obj gcc_startup_mspm0l130x_gcc.obj $(patsubst %.c,%.obj,$(notdir $(SYSCFG_C_FILES)))
NAME = gc_simple_messagepack

CFLAGS += -I.. \
    -I. \
    $(addprefix @,$(SYSCFG_OPT_FILES)) \
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

LFLAGS += "-L$(MSPM0_SDK_INSTALL_DIR)/source/ti/gui_composer/lib/gcc/m0p" \
    -l:gui_composer.a \
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

.INTERMEDIATE: syscfg
$(SYSCFG_FILES): syscfg
	@ echo generation complete

syscfg: ../gc_simple_messagepack.syscfg
	@ echo Generating configuration files...
	@ $(SYSCFG_CMD_STUB) --output $(@D) $<

define C_RULE
$(basename $(notdir $(1))).obj: $(1) $(SYSCFG_H_FILES)
	@ echo Building $$@
	@ $(CC) $(CFLAGS) $$< -c -o $$@
endef

$(foreach c_file,$(SYSCFG_C_FILES),$(eval $(call C_RULE,$(c_file))))

callbacks_mpack.obj: ../callbacks_mpack.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

MSP_GUI_GUI_mpack.obj: ../MSP_GUI/GUI_mpack.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

MSP_GUI_GUIComm_mpack.obj: ../MSP_GUI/GUIComm_mpack.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

HAL_UART_M0L1306.obj: ../HAL_UART_M0L1306.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

main.obj: ../main.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

gcc_startup_mspm0l130x_gcc.obj: ../gcc/startup_mspm0l130x_gcc.c $(SYSCFG_H_FILES)
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
	@ $(RM) $(SYSCFG_FILES)> $(DEVNULL) 2>&1
