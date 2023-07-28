# rules for building
all: release debug

# the following content will be active only if a 'freertos' subdir exists
ifeq ($(wildcard freertos), freertos)

.PHONY: freertos-release freertos-debug

# rules for building
release: freertos-release
debug: freertos-debug

freertos-release:
	@ $(MAKE) -C freertos release

freertos-debug:

# rules for cleaning
clean-release: clean-freertos-release

clean-freertos-release:
	@ $(MAKE) -C freertos clean-release

endif
