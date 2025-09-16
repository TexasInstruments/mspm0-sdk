# ---------------------------------------
# Compiler Flags
# ---------------------------------------
CFLAGS += \
  -MD \
  -ggdb \
  -fdata-sections \
  -ffunction-sections \
  -fno-strict-aliasing \
  -Wall \
  -Wextra \
  -Werror \
  -Wfatal-errors \
  -Wdouble-promotion \
  -Wstrict-prototypes \
  -Wstrict-overflow \
  -Werror-implicit-function-declaration \
  -Wfloat-equal \
  -Wundef \
  -Wshadow \
  -Wwrite-strings \
  -Wsign-compare \
  -Wmissing-format-attribute \
  -Wunreachable-code \
  -Wcast-align \
  -Wcast-function-type \
  -Wcast-qual \
  -Wnull-dereference \
  -Wuninitialized \
  -Wunused \
  -Wreturn-type \
  -Wredundant-decls \

#  -Wmissing-prototypes \
# conversion is too strict for most mcu driver, may be disable sign/int/arith-conversion
#  -Wconversion

# Size Optimization as default
CFLAGS_OPTIMIZED ?= -O0

# Debugging/Optimization
ifeq ($(DEBUG), 1)
  CFLAGS += -O0
  NO_LTO = 1
else
  CFLAGS += $(CFLAGS_OPTIMIZED)
endif

# ---------------------------------------
# Linker Flags
# ---------------------------------------
ifneq ($(TOOLCHAIN),ticlang)
LDFLAGS += \
  -Wl,-Map=$@.map \
  -Wl,--cref \
  -Wl,-gc-sections \

else
# TI Clang compiler is being used
LDFLAGS += \
  -Wl,-m=$@.map \

endif

# renesas rx and ti arm clang does not support --print-memory-usage flags
ifneq ($(TOOLCHAIN),ticlang)
ifneq ($(FAMILY),rx)
LDFLAGS += -Wl,--print-memory-usage
endif
endif

ifeq ($(TOOLCHAIN),gcc)
CC_VERSION := $(shell $(CC) -dumpversion)
CC_VERSION_MAJOR = $(firstword $(subst ., ,$(CC_VERSION)))

# from version 12
ifeq ($(strip $(if $(CMDEXE),\
               $(shell if $(CC_VERSION_MAJOR) geq 12 (echo 1) else (echo 0)),\
               $(shell expr $(CC_VERSION_MAJOR) \>= 12))), 1)
LDFLAGS += -Wl,--no-warn-rwx-segment
endif
endif
