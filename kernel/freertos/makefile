# KERNEL_BUILD below selects a specific kernel to build. By default,
# '*' is selected so all kernels are built.
#
# KERNEL_BUILD options supported match directory names contained within the
# builds/ (choose only one for KERNEL_BUILD):
#
KERNEL_BUILD ?= *

# Find subdirectories
RELEASE-TICLANG-SUBDIRS = $(wildcard builds/$(KERNEL_BUILD)/release/ticlang/.)

include ../../imports.mak

RELEASE-SUBDIRS =
ifneq ($(TICLANG_ARMCOMPILER),)
	RELEASE-SUBDIRS += $(RELEASE-TICLANG-SUBDIRS)
endif

.PHONY: release debug $(RELEASE-SUBDIRS) $(DEBUG-SUBDIRS)

# rules for building
all: release

release: $(RELEASE-SUBDIRS)

$(RELEASE-SUBDIRS):
	@ $(MAKE) -C $@

CLEAN-RELEASE-SUBDIRS = $(addprefix clean-,$(RELEASE-SUBDIRS))

.PHONY: clean-release $(CLEAN-RELEASE-SUBDIRS)

# rules for cleaning
clean: clean-release

clean-release: $(CLEAN-RELEASE-SUBDIRS)

$(CLEAN-RELEASE-SUBDIRS):
	@ $(MAKE) -C $(subst clean-,,$@) clean
