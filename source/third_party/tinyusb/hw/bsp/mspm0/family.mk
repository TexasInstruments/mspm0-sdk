SDK_DIR = hw/mcu/ti/mspm0

include $(TOP)/$(BOARD_PATH)/board.mk
CPU_CORE ?= cortex-m0plus

NO_LTO = 1

ifeq ($(MCU_VARIANT), mspm0g511x)
	CFLAGS += -DCFG_TUSB_MCU=OPT_MCU_MSPM0G511X
else ifeq ($(MCU_VARIANT), mspm0g518x)
	CFLAGS += -DCFG_TUSB_MCU=OPT_MCU_MSPM0G518X
endif

# mcu driver cause following warnings
CFLAGS += -Wno-error=cast-qual

CFLAGS_GCC += -Wno-error=format=
LDFLAGS_GCC += --specs=nosys.specs --specs=nano.specs

ifeq ($(TOOLCHAIN),ticlang)
LINKER_FILE_EXT=cmd
else
LINKER_FILE_EXT=lds
endif

LD_FILE = $(SDK_DIR)/ti/devices/msp/m0p/linker_files/${TOOLCHAIN}/${MCU_SPECIFIC}.${LINKER_FILE_EXT}

SRC_C += \
	src/portable/mentor/musb/dcd_musb.c \
	src/portable/mentor/musb/hcd_musb.c \
	$(SDK_DIR)/ti/driverlib/dl_common.c \
	$(SDK_DIR)/system_${MCU_VARIANT}.c \
	$(SDK_DIR)/ti/devices/msp/m0p/startup_system_files/${TOOLCHAIN}/startup_${MCU_VARIANT}_${TOOLCHAIN}.c \

INC += \
	$(TOP)/lib/CMSIS_5/CMSIS/Core/Include \
	$(TOP)/$(SDK_DIR)/ \
	$(TOP)/$(SDK_DIR)/ti \
	$(TOP)/$(SDK_DIR)/ti/devices/msp \
	$(TOP)/$(SDK_DIR)/ti/driverlib \
	$(TOP)/$(BOARD_PATH) \

# For flash-jlink target
JLINK_DEVICE = $(MCU_VARIANT)
