MSPM0_SDK_INSTALL_DIR ?= $(abspath ../../../../../..)

include $(MSPM0_SDK_INSTALL_DIR)/imports.mak

CC = "$(IAR_ARMCOMPILER)/bin/iccarm"
LNK = "$(IAR_ARMCOMPILER)/bin/ilinkarm"

SYSCFG_CMD_STUB = $(SYSCONFIG_TOOL) --compiler iar --product $(MSPM0_SDK_INSTALL_DIR)/.metadata/product.json
SYSCFG_FILES := $(shell $(SYSCFG_CMD_STUB) --listGeneratedFiles --listReferencedFiles --output . ../empty.syscfg)

SYSCFG_C_FILES = $(filter %.c,$(SYSCFG_FILES))
SYSCFG_H_FILES = $(filter %.h,$(SYSCFG_FILES))
SYSCFG_OPT_FILES = $(filter %.opt,$(SYSCFG_FILES))

OBJECTS = empty.obj ti_driverlib_dl_adc12.obj ti_driverlib_dl_aes.obj ti_driverlib_dl_aesadv.obj ti_driverlib_dl_common.obj ti_driverlib_dl_crc.obj ti_driverlib_dl_crcp.obj ti_driverlib_dl_dac12.obj ti_driverlib_dl_dma.obj ti_driverlib_dl_flashctl.obj ti_driverlib_dl_i2c.obj ti_driverlib_dl_keystorectl.obj ti_driverlib_dl_lcd.obj ti_driverlib_dl_lfss.obj ti_driverlib_dl_mathacl.obj ti_driverlib_dl_mcan.obj ti_driverlib_dl_opa.obj ti_driverlib_dl_rtc_common.obj ti_driverlib_dl_spi.obj ti_driverlib_dl_timer.obj ti_driverlib_dl_trng.obj ti_driverlib_dl_uart.obj ti_driverlib_dl_vref.obj ti_driverlib_m0p_dl_interrupt.obj ti_driverlib_m0p_sysctl_dl_sysctl_mspm0c110x.obj ti_driverlib_m0p_sysctl_dl_sysctl_mspm0g1x0x_g3x0x.obj ti_driverlib_m0p_sysctl_dl_sysctl_mspm0l11xx_l13xx.obj ti_driverlib_m0p_sysctl_dl_sysctl_mspm0l122x_l222x.obj iar_startup_mspm0l130x_iar.obj $(patsubst %.c,%.obj,$(notdir $(SYSCFG_C_FILES)))
NAME = empty_driverlib_src

CFLAGS += -I.. \
    -I. \
    $(addprefix -f,$(SYSCFG_OPT_FILES)) \
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

LFLAGS += "$(MSPM0_SDK_INSTALL_DIR)/source/ti/driverlib/lib/iar/m0p/mspm0l11xx_l13xx/driverlib.a" \
    "-L$(MSPM0_SDK_INSTALL_DIR)/source" \
    -L.. \
    --config ../iar/mspm0l1306.icf \
    --map "$(NAME).map" \
    --silent \
    --semihosting=iar_breakpoint \
    --cpu=Cortex-M0+

all: $(NAME).out

.INTERMEDIATE: syscfg
$(SYSCFG_FILES): syscfg
	@ echo generation complete

syscfg: ../empty.syscfg
	@ echo Generating configuration files...
	@ $(SYSCFG_CMD_STUB) --output $(@D) $<

define C_RULE
$(basename $(notdir $(1))).obj: $(1) $(SYSCFG_H_FILES)
	@ echo Building $$@
	@ $(CC) $(CFLAGS) $$< -o $$@
endef

$(foreach c_file,$(SYSCFG_C_FILES),$(eval $(call C_RULE,$(c_file))))

empty.obj: ../empty.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

ti_driverlib_dl_adc12.obj: ../ti/driverlib/dl_adc12.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

ti_driverlib_dl_aes.obj: ../ti/driverlib/dl_aes.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

ti_driverlib_dl_aesadv.obj: ../ti/driverlib/dl_aesadv.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

ti_driverlib_dl_common.obj: ../ti/driverlib/dl_common.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

ti_driverlib_dl_crc.obj: ../ti/driverlib/dl_crc.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

ti_driverlib_dl_crcp.obj: ../ti/driverlib/dl_crcp.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

ti_driverlib_dl_dac12.obj: ../ti/driverlib/dl_dac12.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

ti_driverlib_dl_dma.obj: ../ti/driverlib/dl_dma.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

ti_driverlib_dl_flashctl.obj: ../ti/driverlib/dl_flashctl.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

ti_driverlib_dl_i2c.obj: ../ti/driverlib/dl_i2c.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

ti_driverlib_dl_keystorectl.obj: ../ti/driverlib/dl_keystorectl.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

ti_driverlib_dl_lcd.obj: ../ti/driverlib/dl_lcd.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

ti_driverlib_dl_lfss.obj: ../ti/driverlib/dl_lfss.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

ti_driverlib_dl_mathacl.obj: ../ti/driverlib/dl_mathacl.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

ti_driverlib_dl_mcan.obj: ../ti/driverlib/dl_mcan.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

ti_driverlib_dl_opa.obj: ../ti/driverlib/dl_opa.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

ti_driverlib_dl_rtc_common.obj: ../ti/driverlib/dl_rtc_common.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

ti_driverlib_dl_spi.obj: ../ti/driverlib/dl_spi.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

ti_driverlib_dl_timer.obj: ../ti/driverlib/dl_timer.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

ti_driverlib_dl_trng.obj: ../ti/driverlib/dl_trng.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

ti_driverlib_dl_uart.obj: ../ti/driverlib/dl_uart.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

ti_driverlib_dl_vref.obj: ../ti/driverlib/dl_vref.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

ti_driverlib_m0p_dl_interrupt.obj: ../ti/driverlib/m0p/dl_interrupt.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

ti_driverlib_m0p_sysctl_dl_sysctl_mspm0c110x.obj: ../ti/driverlib/m0p/sysctl/dl_sysctl_mspm0c110x.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

ti_driverlib_m0p_sysctl_dl_sysctl_mspm0g1x0x_g3x0x.obj: ../ti/driverlib/m0p/sysctl/dl_sysctl_mspm0g1x0x_g3x0x.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

ti_driverlib_m0p_sysctl_dl_sysctl_mspm0l11xx_l13xx.obj: ../ti/driverlib/m0p/sysctl/dl_sysctl_mspm0l11xx_l13xx.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

ti_driverlib_m0p_sysctl_dl_sysctl_mspm0l122x_l222x.obj: ../ti/driverlib/m0p/sysctl/dl_sysctl_mspm0l122x_l222x.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

iar_startup_mspm0l130x_iar.obj: ../iar/startup_mspm0l130x_iar.c $(SYSCFG_H_FILES)
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
	@ $(RM) $(SYSCFG_FILES)> $(DEVNULL) 2>&1
