MSPM0_SDK_INSTALL_DIR ?= $(abspath ../../..)
include $(MSPM0_SDK_INSTALL_DIR)/imports.mak

# Find all subdirectories
NORTOS-GCC-SUBDIRS = $(wildcard */*/gcc/.)
NORTOS-IAR-SUBDIRS = $(wildcard */*/iar/.)
NORTOS-TICLANG-SUBDIRS = $(wildcard */*/ticlang/.)

NORTOS-SUBDIRS =
ifneq ($(GCC_ARMCOMPILER),)
	NORTOS-SUBDIRS += $(NORTOS-GCC-SUBDIRS)
endif
ifneq ($(IAR_ARMCOMPILER),)
	NORTOS-SUBDIRS += $(NORTOS-IAR-SUBDIRS)
endif
ifneq ($(TICLANG_ARMCOMPILER),)
	NORTOS-SUBDIRS += $(NORTOS-TICLANG-SUBDIRS)
endif

.PHONY: nortos $(NORTOS-SUBDIRS)

all: nortos

nortos: $(NORTOS-SUBDIRS)

$(NORTOS-SUBDIRS):
	@ $(MAKE) -C $@

CLEAN-NORTOS-SUBDIRS = $(addprefix clean-,$(NORTOS-SUBDIRS))

.PHONY: clean-nortos $(CLEAN-NORTOS-SUBDIRS)

clean: clean-nortos

clean-nortos: $(CLEAN-NORTOS-SUBDIRS)

$(CLEAN-NORTOS-SUBDIRS):
	@ $(MAKE) -C $(subst clean-,,$@) clean
