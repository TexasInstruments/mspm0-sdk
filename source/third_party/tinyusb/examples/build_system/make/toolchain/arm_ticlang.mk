# makefile for Texas Instruments TI ARM Clang toolchain

CC		= tiarmclang
CXX		= $(CC)
AS		= $(CC) -x assembler-with-cpp
LD		= $(CC)

OBJCOPY = tiarmobjcopy
SIZE	= tiarmsize

include ${TOP}/examples/build_system/make/toolchain/gcc_common.mk
