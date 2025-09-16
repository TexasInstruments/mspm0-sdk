MSPM0_SDK_INSTALL_DIR ?= $(abspath ../../../../../..)

include $(MSPM0_SDK_INSTALL_DIR)/imports.mak

CC = "$(TICLANG_ARMCOMPILER)/bin/tiarmclang"
LNK = "$(TICLANG_ARMCOMPILER)/bin/tiarmclang"

SYSCONFIG_GUI_TOOL = $(dir $(SYSCONFIG_TOOL))sysconfig_gui$(suffix $(SYSCONFIG_TOOL))
SYSCFG_CMD_STUB = $(SYSCONFIG_TOOL) --compiler ticlang --product $(MSPM0_SDK_INSTALL_DIR)/.metadata/product.json
SYSCFG_GUI_CMD_STUB = $(SYSCONFIG_GUI_TOOL) --compiler ticlang --product $(MSPM0_SDK_INSTALL_DIR)/.metadata/product.json
SYSCFG_FILES := $(shell $(SYSCFG_CMD_STUB) --listGeneratedFiles --listReferencedFiles --output . ../host_device_info.syscfg)

SYSCFG_C_FILES = $(filter %.c,$(SYSCFG_FILES))
SYSCFG_H_FILES = $(filter %.h,$(SYSCFG_FILES))
SYSCFG_OPT_FILES = $(filter %.opt,$(SYSCFG_FILES))

OBJECTS = main.obj third_party_tinyusb_hw_bsp_board.obj third_party_tinyusb_hw_bsp_mspm0_family.obj third_party_tinyusb_hw_mcu_ti_mspm0_system_mspm0g511x.obj third_party_tinyusb_hw_mcu_ti_mspm0_system_mspm0g518x.obj third_party_tinyusb_src_class_audio_audio_device.obj third_party_tinyusb_src_class_bth_bth_device.obj third_party_tinyusb_src_class_cdc_cdc_device.obj third_party_tinyusb_src_class_cdc_cdc_host.obj third_party_tinyusb_src_class_cdc_cdc_rndis_host.obj third_party_tinyusb_src_class_dfu_dfu_device.obj third_party_tinyusb_src_class_dfu_dfu_rt_device.obj third_party_tinyusb_src_class_hid_hid_device.obj third_party_tinyusb_src_class_hid_hid_host.obj third_party_tinyusb_src_class_midi_midi_device.obj third_party_tinyusb_src_class_msc_msc_device.obj third_party_tinyusb_src_class_msc_msc_host.obj third_party_tinyusb_src_class_net_ecm_rndis_device.obj third_party_tinyusb_src_class_net_ncm_device.obj third_party_tinyusb_src_class_usbtmc_usbtmc_device.obj third_party_tinyusb_src_class_vendor_vendor_device.obj third_party_tinyusb_src_class_vendor_vendor_host.obj third_party_tinyusb_src_class_video_video_device.obj third_party_tinyusb_src_common_tusb_fifo.obj third_party_tinyusb_src_device_usbd.obj third_party_tinyusb_src_device_usbd_control.obj third_party_tinyusb_src_host_hub.obj third_party_tinyusb_src_host_usbh.obj third_party_tinyusb_src_portable_mentor_musb_dcd_musb.obj third_party_tinyusb_src_portable_mentor_musb_hcd_musb.obj third_party_tinyusb_src_tusb.obj third_party_tinyusb_lib_fatfs_source_ff.obj third_party_tinyusb_lib_fatfs_source_ffsystem.obj third_party_tinyusb_lib_fatfs_source_ffunicode.obj $(patsubst %.c,%.obj,$(notdir $(SYSCFG_C_FILES)))
NAME = host_device_info

CFLAGS += -I.. \
    -I. \
    $(addprefix @,$(SYSCFG_OPT_FILES)) \
    -g \
    -mthumb \
    -mcpu=cortex-m0plus \
    -mfloat-abi=soft \
    -fsingle-precision-constant \
    -MD \
    -fdata-sections \
    -ffunction-sections \
    -fno-strict-aliasing \
    -Wextra \
    -Wfatal-errors \
    -Wdouble-promotion \
    -Wstrict-prototypes \
    -Wstrict-overflow \
    -Werrorimplicit-function-declaration \
    -Wfloat-equal \
    -Wshadow \
    -Wwrite-strings \
    -Wsign-compare \
    -Wmissing-format-attribute \
    -Wunreachable-code \
    -Wnull-dereference \
    -Wuninitialized \
    -Wreturn-type \
    -Wredundant-decls \
    -O0 \
    -Wno-error=format \
    @device.opt \
    "-I$(MSPM0_SDK_INSTALL_DIR)/source/third_party/CMSIS/Core/Include" \
    -I../third_party/tinyusb/src \
    -I../third_party/tinyusb/hw \
    -I../third_party/tinyusb/lib/fatfs/source \
    -I../third_party/tinyusb/lib/embedded-cli \
    -I../third_party/tinyusb/hw/bsp/mspm0 \
    -I${COM_TI_MSPM0_SDK_INSTALL_DIR}/source \
    -I${COM_TI_MSPM0_SDK_INSTALL_DIR}/source/ti \
    -I${COM_TI_MSPM0_SDK_INSTALL_DIR}/source/ti/devices/msp \
    "-I$(MSPM0_SDK_INSTALL_DIR)/source" \
    -gdwarf-3 \
    -march=thumbv6m

LFLAGS += -Wl,-Map=.map \
    -Wl,--unused_section_elimination=on \
    -Wl,--rom_model \
    -Wl,--warn_sections \
    -g \
    -mthumb \
    -mcpu=cortex-m0plus \
    -mfloat-abi=soft \
    "-L$(TICLANG_ARMCOMPILER)/arm-none-eabi/lib/thumb/v6-m/nofp" \
    -MD \
    -ldevice.cmd.genlibs \
    "-L$(MSPM0_SDK_INSTALL_DIR)/source" \
    -L.. \
    ../ticlang/device_linker.cmd \
    "-Wl,-m,$(NAME).map" \
    "-L$(TICLANG_ARMCOMPILER)/lib" \
    -llibc.a

all: $(NAME).out

.INTERMEDIATE: syscfg
$(SYSCFG_FILES): syscfg
	@ echo generation complete

syscfg: ../host_device_info.syscfg
	@ echo Generating configuration files...
	@ $(SYSCFG_CMD_STUB) --output $(@D) $<


# Helpful hint that the user needs to use a standalone SysConfig installation
$(SYSCONFIG_GUI_TOOL):
	$(error $(dir $(SYSCONFIG_TOOL)) does not contain the GUI framework \
        necessary to launch the SysConfig GUI.  Please set SYSCONFIG_TOOL \
        (in your SDK's imports.mak) to a standalone SysConfig installation \
        rather than one inside CCS)

syscfg-gui: ../host_device_info.syscfg $(SYSCONFIG_GUI_TOOL)
	@ echo Opening SysConfig GUI
	@ $(SYSCFG_GUI_CMD_STUB) $<

define C_RULE
$(basename $(notdir $(1))).obj: $(1) $(SYSCFG_H_FILES)
	@ echo Building $$@
	@ $(CC) $(CFLAGS) -c $$< -o $$@
endef

$(foreach c_file,$(SYSCFG_C_FILES),$(eval $(call C_RULE,$(c_file))))

main.obj: ../main.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

third_party_tinyusb_hw_bsp_board.obj: ../third_party/tinyusb/hw/bsp/board.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

third_party_tinyusb_hw_bsp_mspm0_family.obj: ../third_party/tinyusb/hw/bsp/mspm0/family.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

third_party_tinyusb_hw_mcu_ti_mspm0_system_mspm0g511x.obj: ../third_party/tinyusb/hw/mcu/ti/mspm0/system_mspm0g511x.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

third_party_tinyusb_hw_mcu_ti_mspm0_system_mspm0g518x.obj: ../third_party/tinyusb/hw/mcu/ti/mspm0/system_mspm0g518x.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

third_party_tinyusb_src_class_audio_audio_device.obj: ../third_party/tinyusb/src/class/audio/audio_device.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

third_party_tinyusb_src_class_bth_bth_device.obj: ../third_party/tinyusb/src/class/bth/bth_device.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

third_party_tinyusb_src_class_cdc_cdc_device.obj: ../third_party/tinyusb/src/class/cdc/cdc_device.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

third_party_tinyusb_src_class_cdc_cdc_host.obj: ../third_party/tinyusb/src/class/cdc/cdc_host.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

third_party_tinyusb_src_class_cdc_cdc_rndis_host.obj: ../third_party/tinyusb/src/class/cdc/cdc_rndis_host.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

third_party_tinyusb_src_class_dfu_dfu_device.obj: ../third_party/tinyusb/src/class/dfu/dfu_device.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

third_party_tinyusb_src_class_dfu_dfu_rt_device.obj: ../third_party/tinyusb/src/class/dfu/dfu_rt_device.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

third_party_tinyusb_src_class_hid_hid_device.obj: ../third_party/tinyusb/src/class/hid/hid_device.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

third_party_tinyusb_src_class_hid_hid_host.obj: ../third_party/tinyusb/src/class/hid/hid_host.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

third_party_tinyusb_src_class_midi_midi_device.obj: ../third_party/tinyusb/src/class/midi/midi_device.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

third_party_tinyusb_src_class_msc_msc_device.obj: ../third_party/tinyusb/src/class/msc/msc_device.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

third_party_tinyusb_src_class_msc_msc_host.obj: ../third_party/tinyusb/src/class/msc/msc_host.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

third_party_tinyusb_src_class_net_ecm_rndis_device.obj: ../third_party/tinyusb/src/class/net/ecm_rndis_device.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

third_party_tinyusb_src_class_net_ncm_device.obj: ../third_party/tinyusb/src/class/net/ncm_device.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

third_party_tinyusb_src_class_usbtmc_usbtmc_device.obj: ../third_party/tinyusb/src/class/usbtmc/usbtmc_device.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

third_party_tinyusb_src_class_vendor_vendor_device.obj: ../third_party/tinyusb/src/class/vendor/vendor_device.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

third_party_tinyusb_src_class_vendor_vendor_host.obj: ../third_party/tinyusb/src/class/vendor/vendor_host.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

third_party_tinyusb_src_class_video_video_device.obj: ../third_party/tinyusb/src/class/video/video_device.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

third_party_tinyusb_src_common_tusb_fifo.obj: ../third_party/tinyusb/src/common/tusb_fifo.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

third_party_tinyusb_src_device_usbd.obj: ../third_party/tinyusb/src/device/usbd.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

third_party_tinyusb_src_device_usbd_control.obj: ../third_party/tinyusb/src/device/usbd_control.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

third_party_tinyusb_src_host_hub.obj: ../third_party/tinyusb/src/host/hub.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

third_party_tinyusb_src_host_usbh.obj: ../third_party/tinyusb/src/host/usbh.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

third_party_tinyusb_src_portable_mentor_musb_dcd_musb.obj: ../third_party/tinyusb/src/portable/mentor/musb/dcd_musb.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

third_party_tinyusb_src_portable_mentor_musb_hcd_musb.obj: ../third_party/tinyusb/src/portable/mentor/musb/hcd_musb.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

third_party_tinyusb_src_tusb.obj: ../third_party/tinyusb/src/tusb.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

third_party_tinyusb_lib_fatfs_source_ff.obj: ../third_party/tinyusb/lib/fatfs/source/ff.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

third_party_tinyusb_lib_fatfs_source_ffsystem.obj: ../third_party/tinyusb/lib/fatfs/source/ffsystem.c $(SYSCFG_H_FILES)
	@ echo Building $@
	@ $(CC) $(CFLAGS) -c $< -o $@

third_party_tinyusb_lib_fatfs_source_ffunicode.obj: ../third_party/tinyusb/lib/fatfs/source/ffunicode.c $(SYSCFG_H_FILES)
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
