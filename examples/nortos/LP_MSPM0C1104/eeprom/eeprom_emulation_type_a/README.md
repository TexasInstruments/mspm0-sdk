## Example Summary

This example demonstrates the use of Type A EEPROM emulation to store user data.

Type A EEPROM Emulation maximizes the use of Flash memory when writing blocks of non-volatile data.
For more programming granularity, please check the Type B implementation.

## Peripherals & Pin Assignments
- FlashCtl

## BoosterPacks, Board Resources & Jumper Settings
N/A


### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0C1104 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#non-sysconfig-compatible-project-migration)
for information about migrating to other MSPM0 devices.

## Example Usage

Compile, load and run the example.
After initialization, part of the Flash area will be used as emulated EEPROM.
In the application, the data is saved to the emulated EEPROM 9 times, with the
9th iteration erasing the sector for FLASH which had been written to.
The application will stop at breakpoints so the user can verify the values.
