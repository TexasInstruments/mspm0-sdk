## Example Summary

This example demonstrates a more detailed use case of Type B EEPROM emulation to store user data.

Type B EEPROM Emulation maximizes the lifetime of Flash memory when writing individual words of non-volatile data.
Please check the Type A implementation when writing blocks of data.

## Peripherals & Pin Assignments
- FlashCtl

## BoosterPacks, Board Resources & Jumper Settings
N/A


### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0G3507 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#non-sysconfig-compatible-project-migration)
for information about migrating to other MSPM0 devices.

## Example Usage

Compile, load and run the example.
After initialization, part of the Flash area will be used as emulated EEPROM.
In the application, the data is saved to the emulated EEPROM in three different
groups showing how to write into EEPROM three unique ways with descriptions of
the memory as commands are executed.