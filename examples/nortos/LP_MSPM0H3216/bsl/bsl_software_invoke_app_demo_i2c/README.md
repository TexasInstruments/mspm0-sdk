## Example Summary

This example demonstrates an application request to reset the device and jump to
the ROM bootloader (BSL).

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA14 | Standard Input with internal pull-up |
| GPIOA | PA0 | Standard Output |
| GPIOB | PB20 | Standard Output |
| SYSCTL |  |  |
| I2C0 | PA30 | I2C Serial Data line (SDA) |
| I2C0 | PA27 | I2C Serial Clock line (SCL) |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0H3216](https://www.ti.com/tool/LP-MSPM0H3216) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA14 | GPIOA | PA14 | N/A | PA14 is connected to S2 button and connect to GND when push the button |
| PA0 | GPIOA | PA0 | J4_1 | PA0 can be connected to LED1<br><ul><li>`J4 ON` Connect to LED1|
| PB20 | GPIOB | PB20 | J7_1 | PB20 can be connected to LED2 blue<br><ul><li>`J7 ON` Connect to LED2 blue|
| PA30 | I2C0 | SDA | J1, J9_1 | PA30 requires pull-up external, it also used to control LED2 Green that the jumper on J9 need to be removed.<br><ul><li>`J9 OFF` Disconnect from LED2 Green |
| PA27 | I2C0 | SCL | J1 | PA37 requires pull-up external|
| PA20 | DEBUGSS | SWCLK | J2_13 | PA20 is used by SWD during debugging<br><ul><li>`J101 15:16 ON` Connect to XDS-110 SWCLK while debugging<br><li>`J101 15:16 OFF` Disconnect from XDS-110 SWCLK if using pin in application |
| PA19 | DEBUGSS | SWDIO | J2_17 | PA19 is used by SWD during debugging<br><ul><li>`J101 13:14 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J101 13:14 OFF` Disconnect from XDS-110 SWDIO if using pin in application |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0H3216 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#manual-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0H3216 User's Guide](https://www.ti.com/lit/slau951).

## Example Usage
This demo's firmware is used for the BSL host device. 
The example will begin toggling LED1 while waiting for the user to press
the S2 button or send 0x22 from host to trigger BSL.

When S2 is pressed or 0x22 received, the application will clear LED1 and blink LED2.blue. 
The device will reset with BOOTLOADERENTRY reset level. This will run the
bootcode, which will check if there is a BSL request from the application, and then
invoke the BSL. At this point, both LED1 and LED2.Blue will be off.
Once the BSL has been invoked, the user can send commands to the BSL through the
external BSL Host.

To return to application code, the user can trigger a POR/BOR on the device, or use
the BSL Host to send the application start command to the device.
