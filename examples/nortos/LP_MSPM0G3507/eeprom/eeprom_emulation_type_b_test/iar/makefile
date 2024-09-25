MSPM0_SDK_INSTALL_DIR ?= $(abspath ../../../../../..)

include $(MSPM0_SDK_INSTALL_DIR)/imports.mak

CC = "$(IAR_ARMCOMPILER)/bin/iccarm"
LNK = "$(IAR_ARMCOMPILER)/bin/ilinkarm"

OBJECTS = main.obj ti_eeprom_emulation_type_a_eeprom_emulation_type_a.obj ti_eeprom_emulation_type_b_eeprom_emulation_type_b.obj ti_msp_dl_config.obj iar_startup_mspm0g350x_iar.obj
NAME = eeprom_emulation_type_b_test

CFLAGS += -I.. \
    -D__MSPM0G3507__ \
    -Om \
    "-I$(MSPM0_SDK_INSTALL_DIR)/source/third_party/CMSIS/Core/Include" \
    "-I$(MSPM0_SDK_INSTALL_DIR)/source" \
    --debug \
    --silent \
    -e \
    --aeabi \
    --thumb \
    --diag_suppress=Pa050 \
    --cpu=Cortex-M0+ \
    --vla

LFLAGS += "$(MSPM0_SDK_INSTALL_DIR)/source/ti/driverlib/lib/iar/m0p/mspm0g1x0x_g3x0x/driverlib.a" \
    "-L$(MSPM0_SDK_INSTALL_DIR)/source" \
    -L.. \
    --config ../iar/mspm0g3507.icf \
    --map "$(NAME).map" \
    --silent \
    --semihosting=iar_breakpoint \
    --cpu=Cortex-M0+

all: $(NAME).out

main.obj: ../main.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

ti_eeprom_emulation_type_a_eeprom_emulation_type_a.obj: ../ti/eeprom/emulation_type_a/eeprom_emulation_type_a.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

ti_eeprom_emulation_type_b_eeprom_emulation_type_b.obj: ../ti/eeprom/emulation_type_b/eeprom_emulation_type_b.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

ti_msp_dl_config.obj: ../ti_msp_dl_config.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

iar_startup_mspm0g350x_iar.obj: ../iar/startup_mspm0g350x_iar.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

$(NAME).out: $(OBJECTS)
	@ echo linking $@
	@ $(LNK)  $(OBJECTS)  $(LFLAGS) -o $(NAME).out

clean:
	@ echo Cleaning...
	@ $(RM) $(OBJECTS) > $(DEVNULL) 2>&1
	@ $(RM) $(NAME).out > $(DEVNULL) 2>&1
	@ $(RM) $(NAME).map > $(DEVNULL) 2>&1
