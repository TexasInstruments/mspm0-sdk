MSPM0_SDK_INSTALL_DIR ?= $(abspath ../../..)
include $(MSPM0_SDK_INSTALL_DIR)/imports.mak

# Find all subdirectories
FREERTOS-TICLANG-SUBDIRS = $(wildcard */*/freertos/ticlang/.)

FREERTOS-SUBDIRS =
ifneq ($(TICLANG_ARMCOMPILER),)
	FREERTOS-SUBDIRS += $(FREERTOS-TICLANG-SUBDIRS)
endif

.PHONY: freertos $(FREERTOS-SUBDIRS)

all: freertos

freertos: $(FREERTOS-SUBDIRS)

$(FREERTOS-SUBDIRS):
	@ $(MAKE) -C $@

CLEAN-FREERTOS-SUBDIRS = $(addprefix clean-,$(FREERTOS-SUBDIRS))

.PHONY: clean-freertos $(CLEAN-FREERTOS-SUBDIRS)

clean: clean-freertos

clean-freertos: $(CLEAN-FREERTOS-SUBDIRS)

$(CLEAN-FREERTOS-SUBDIRS):
	@ $(MAKE) -C $(subst clean-,,$@) clean
