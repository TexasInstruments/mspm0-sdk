MSPM0_SDK_INSTALL_DIR ?= $(abspath ../../../../../..)

include $(MSPM0_SDK_INSTALL_DIR)/imports.mak

CC = "$(TICLANG_ARMCOMPILER)/bin/tiarmclang"
LNK = "$(TICLANG_ARMCOMPILER)/bin/tiarmclang"

SYSCFG_CMD_STUB = $(SYSCONFIG_TOOL) --compiler ticlang --product $(MSPM0_SDK_INSTALL_DIR)/.metadata/product.json
SYSCFG_FILES := $(shell $(SYSCFG_CMD_STUB) --listGeneratedFiles --listReferencedFiles --output . ../boot_application.syscfg)

SYSCFG_C_FILES = $(filter %.c,$(SYSCFG_FILES))
SYSCFG_H_FILES = $(filter %.h,$(SYSCFG_FILES))
SYSCFG_OPT_FILES = $(filter %.opt,$(SYSCFG_FILES))

OBJECTS = flash_map_backend_flash_map_backend.obj mcuboot_config_trace.obj boot_application.obj boot_keys.obj third_party_mcuboot_boot_bootutil_src_boot_record.obj third_party_mcuboot_boot_bootutil_src_bootutil_misc.obj third_party_mcuboot_boot_bootutil_src_bootutil_public.obj third_party_mcuboot_boot_bootutil_src_caps.obj third_party_mcuboot_boot_bootutil_src_encrypted.obj third_party_mcuboot_boot_bootutil_src_fault_injection_hardening.obj third_party_mcuboot_boot_bootutil_src_fault_injection_hardening_delay_rng_mbedtls.obj third_party_mcuboot_boot_bootutil_src_image_ec.obj third_party_mcuboot_boot_bootutil_src_image_ec256.obj third_party_mcuboot_boot_bootutil_src_image_ed25519.obj third_party_mcuboot_boot_bootutil_src_image_rsa.obj third_party_mcuboot_boot_bootutil_src_image_validate.obj third_party_mcuboot_boot_bootutil_src_loader.obj third_party_mcuboot_boot_bootutil_src_swap_misc.obj third_party_mcuboot_boot_bootutil_src_swap_move.obj third_party_mcuboot_boot_bootutil_src_swap_scratch.obj third_party_mcuboot_boot_bootutil_src_tlv.obj third_party_mcuboot_ext_mbedtls-asn1_src_asn1parse.obj third_party_mcuboot_ext_mbedtls-asn1_src_platform_util.obj third_party_mcuboot_ext_msp-crypto_msp_crypto.obj ti_driverlib_dl_flashctl.obj ticlang_startup_mspm0l130x_ticlang.obj $(patsubst %.c,%.obj,$(notdir $(SYSCFG_C_FILES)))
NAME = boot_application

CFLAGS += -I.. \
    -I. \
    $(addprefix @,$(SYSCFG_OPT_FILES)) \
    -D__MSPM0L1306__ \
    -DEXCLUDE_TRACE \
    -Oz \
    "-I$(MSPM0_SDK_INSTALL_DIR)/source/third_party/mcuboot/boot/bootutil/include" \
    "-I$(MSPM0_SDK_INSTALL_DIR)/source/third_party/mcuboot/ext/mbedtls-asn1/include" \
    "-I$(MSPM0_SDK_INSTALL_DIR)/source/third_party/mcuboot/ext/" \
    "-I$(MSPM0_SDK_INSTALL_DIR)/source/ti/boot_manager/ecc/include" \
    "-I$(MSPM0_SDK_INSTALL_DIR)/source/ti/boot_manager/sha2sw/inc" \
    "-I$(MSPM0_SDK_INSTALL_DIR)/source/third_party/CMSIS/Core/Include" \
    "-I$(MSPM0_SDK_INSTALL_DIR)/source" \
    -gdwarf-3 \
    -mcpu=cortex-m0plus \
    -march=thumbv6m \
    -mfloat-abi=soft \
    -mthumb

LFLAGS += "-l$(MSPM0_SDK_INSTALL_DIR)/source/ti/boot_manager/lib/ticlang/m0p/bimsupport.a" \
    -Wl,--stack_size=256 \
    -Wl,--heap_size=128 \
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

.INTERMEDIATE: syscfg
$(SYSCFG_FILES): syscfg
	@ echo generation complete

syscfg: ../boot_application.syscfg
	@ echo Generating configuration files...
	@ $(SYSCFG_CMD_STUB) --output $(@D) $<

define C_RULE
$(basename $(notdir $(1))).obj: $(1) $(SYSCFG_H_FILES)
	@ echo Building $$@
	@ $(CC) $(CFLAGS) -c $$< -o $$@
endef

$(foreach c_file,$(SYSCFG_C_FILES),$(eval $(call C_RULE,$(c_file))))

flash_map_backend_flash_map_backend.obj: ../flash_map_backend/flash_map_backend.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

mcuboot_config_trace.obj: ../mcuboot_config/trace.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

boot_application.obj: ../boot_application.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

boot_keys.obj: ../boot_keys.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

third_party_mcuboot_boot_bootutil_src_boot_record.obj: ../third_party/mcuboot/boot/bootutil/src/boot_record.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

third_party_mcuboot_boot_bootutil_src_bootutil_misc.obj: ../third_party/mcuboot/boot/bootutil/src/bootutil_misc.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

third_party_mcuboot_boot_bootutil_src_bootutil_public.obj: ../third_party/mcuboot/boot/bootutil/src/bootutil_public.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

third_party_mcuboot_boot_bootutil_src_caps.obj: ../third_party/mcuboot/boot/bootutil/src/caps.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

third_party_mcuboot_boot_bootutil_src_encrypted.obj: ../third_party/mcuboot/boot/bootutil/src/encrypted.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

third_party_mcuboot_boot_bootutil_src_fault_injection_hardening.obj: ../third_party/mcuboot/boot/bootutil/src/fault_injection_hardening.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

third_party_mcuboot_boot_bootutil_src_fault_injection_hardening_delay_rng_mbedtls.obj: ../third_party/mcuboot/boot/bootutil/src/fault_injection_hardening_delay_rng_mbedtls.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

third_party_mcuboot_boot_bootutil_src_image_ec.obj: ../third_party/mcuboot/boot/bootutil/src/image_ec.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

third_party_mcuboot_boot_bootutil_src_image_ec256.obj: ../third_party/mcuboot/boot/bootutil/src/image_ec256.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

third_party_mcuboot_boot_bootutil_src_image_ed25519.obj: ../third_party/mcuboot/boot/bootutil/src/image_ed25519.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

third_party_mcuboot_boot_bootutil_src_image_rsa.obj: ../third_party/mcuboot/boot/bootutil/src/image_rsa.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

third_party_mcuboot_boot_bootutil_src_image_validate.obj: ../third_party/mcuboot/boot/bootutil/src/image_validate.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

third_party_mcuboot_boot_bootutil_src_loader.obj: ../third_party/mcuboot/boot/bootutil/src/loader.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

third_party_mcuboot_boot_bootutil_src_swap_misc.obj: ../third_party/mcuboot/boot/bootutil/src/swap_misc.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

third_party_mcuboot_boot_bootutil_src_swap_move.obj: ../third_party/mcuboot/boot/bootutil/src/swap_move.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

third_party_mcuboot_boot_bootutil_src_swap_scratch.obj: ../third_party/mcuboot/boot/bootutil/src/swap_scratch.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

third_party_mcuboot_boot_bootutil_src_tlv.obj: ../third_party/mcuboot/boot/bootutil/src/tlv.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

third_party_mcuboot_ext_mbedtls-asn1_src_asn1parse.obj: ../third_party/mcuboot/ext/mbedtls-asn1/src/asn1parse.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

third_party_mcuboot_ext_mbedtls-asn1_src_platform_util.obj: ../third_party/mcuboot/ext/mbedtls-asn1/src/platform_util.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

third_party_mcuboot_ext_msp-crypto_msp_crypto.obj: ../third_party/mcuboot/ext/msp-crypto/msp_crypto.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

ti_driverlib_dl_flashctl.obj: ../ti/driverlib/dl_flashctl.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

ticlang_startup_mspm0l130x_ticlang.obj: ../ticlang/startup_mspm0l130x_ticlang.c $(SYSCFG_H_FILES)
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
	@ $(RM) $(SYSCFG_FILES)> $(DEVNULL) 2>&1
