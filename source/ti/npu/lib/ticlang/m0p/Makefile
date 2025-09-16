SDK_INSTALL_DIR ?= $(abspath ../../../../../..)

include $(SDK_INSTALL_DIR)/imports.mak

CC = "$(TICLANG_ARMCOMPILER)/bin/tiarmclang"
AR = "$(TICLANG_ARMCOMPILER)/bin/tiarmar"

NAME = npu

OBJ_DIR = ../../../obj/ticlang/m0p/npu
SRC_DIR = ../../..

CFLAGS = -I$(TICLANG_ARMCOMPILER)/include "-I$(SDK_INSTALL_DIR)/source/third_party/CMSIS/Core/Include" "-I$(SDK_INSTALL_DIR)/source" -mcpu=cortex-m0plus -march=armv6-m -mtune=cortex-m0plus -mthumb -mfloat-abi=soft -D__MSPM0G5187__ -c -g -gdwarf-3 -O2
AFLAGS = rc
ASMFLAGS = -I$(TICLANG_ARMCOMPILER)/include 

OBJECTS = $(OBJ_DIR)/npu.o

all: $(NAME).a

$(NAME).a: $(OBJECTS)
	@ echo Archiving $@
	@ $(AR) $(AFLAGS) $@ $^

$(OBJ_DIR)/npu.o: $(SRC_DIR)/npu.c
	@ echo Building $@
	@ mkdir -p $(dir $@)
	@ $(CC) $(CFLAGS) $< -o $@

clean:
	@ echo Cleaning...
	@ $(RM) $(OBJECTS) > $(DEVNULL) 2>&1
	@ $(RM) $(NAME).a > $(DEVNULL) 2>&1
