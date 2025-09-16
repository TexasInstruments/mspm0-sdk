ifeq ($(TOOLCHAIN),gcc)
  CFLAGS += \
    -mthumb \
    -mcpu=cortex-m0plus \
    -mfloat-abi=soft \

else ifeq ($(TOOLCHAIN),clang)
  CFLAGS += \
	  --target=arm-none-eabi \
	  -mcpu=cortex-m0plus \

else ifeq ($(TOOLCHAIN),ticlang)
  CFLAGS += \
	  --target=arm-ti-none-eabi \
	  -mcpu=cortex-m0plus \
	  -mfloat-abi=soft \
	  -march=thumbv6m \
	  -mlittle-endian \
	  -mthumb \

else ifeq ($(TOOLCHAIN),iar)
  # IAR Flags
  CFLAGS += --cpu cortex-m0+
  ASFLAGS += --cpu cortex-m0+

else
  $(error "TOOLCHAIN is not supported")
endif

# For freeRTOS port source
FREERTOS_PORTABLE_SRC ?= $(FREERTOS_PORTABLE_PATH)/ARM_CM0
