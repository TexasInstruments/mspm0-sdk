## Example Summary

This example demonstrates the use of Type B EEPROM emulation to store user data.

Type B EEPROM Emulation maximizes the lifetime of Flash memory when writing individual words of non-volatile data.
Please check the Type A implementation when writing blocks of data.

## Peripherals & Pin Assignments
- FlashCtl

## BoosterPacks, Board Resources & Jumper Settings
N/A


### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0L1306 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#non-sysconfig-compatible-project-migration)
for information about migrating to other MSPM0 devices.

## Example Usage

Compile, load and run the example.
After initialization, part of the Flash area will be used as emulated EEPROM.
In the application, the data is continuously saved to the emulated EEPROM
The application will stop at breakpoints so the user can verify the values.
