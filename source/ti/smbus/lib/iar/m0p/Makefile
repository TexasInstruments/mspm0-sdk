SDK_INSTALL_DIR ?= $(abspath ../../../../../..)

include $(SDK_INSTALL_DIR)/imports.mak

CC  = "$(IAR_ARMCOMPILER)/bin/iccarm"
ASM = "$(IAR_ARMCOMPILER)/bin/iasmarm"
AR  = "$(IAR_ARMCOMPILER)/bin/iarchive"

NAME = smbus

OBJ_DIR = ../../../obj/iar/m0p/smbus
SRC_DIR = ../../..

CFLAGS = "-I$(IAR_ARMCOMPILER)/inc/c" "-I$(SDK_INSTALL_DIR)/source/third_party/CMSIS/Core/Include" "-I$(SDK_INSTALL_DIR)/source" --cpu Cortex-M0+ -D__MSPM0G3507__ --aeabi --guard_calls --endian=little --thumb --silent -e --use_unix_directory_separators --diag_suppress=Pa050,Go005
AFLAGS = 
ASMFLAGS = "-I$(IAR_ARMCOMPILER)/inc/c" 

OBJECTS = $(OBJ_DIR)/smbus.o $(OBJ_DIR)/smbus_nwk.o $(OBJ_DIR)/smbus_phy.o

all: $(NAME).a

$(NAME).a: $(OBJECTS)
	@ echo Archiving $@
	@ $(AR) $(AFLAGS) $@ $^

$(OBJ_DIR)/smbus.o: $(SRC_DIR)/smbus.c
	@ echo Building $@
	@ mkdir -p $(dir $@)
	@ $(CC) $(CFLAGS) $< -o $@

$(OBJ_DIR)/smbus_nwk.o: $(SRC_DIR)/smbus_nwk.c
	@ echo Building $@
	@ mkdir -p $(dir $@)
	@ $(CC) $(CFLAGS) $< -o $@

$(OBJ_DIR)/smbus_phy.o: $(SRC_DIR)/smbus_phy.c
	@ echo Building $@
	@ mkdir -p $(dir $@)
	@ $(CC) $(CFLAGS) $< -o $@

clean:
	@ echo Cleaning...
	@ $(RM) $(OBJECTS) > $(DEVNULL) 2>&1
	@ $(RM) $(NAME).a > $(DEVNULL) 2>&1
