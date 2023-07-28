## Example Summary

This example demonstrates the use of Type B EEPROM emulation to store user data.

Type B EEPROM Emulation maximizes the lifetime of Flash memory when writing individual words of non-volatile data.
Please check the Type A implementation when writing blocks of data.

## Peripherals & Pin Assignments
- FlashCtl

## BoosterPacks, Board Resources & Jumper Settings
N/A

## Example Usage

Compile, load and run the example.
After initialization, part of the Flash area will be used as emulated EEPROM.
In the application, the data is continuously saved to the emulated EEPROM
The application will stop at breakpoints so the user can verify the values.
