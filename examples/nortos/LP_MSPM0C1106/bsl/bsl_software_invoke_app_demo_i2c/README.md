## Example Summary

This example demonstrates an application request to reset the device and jump to
the Flash bootloader (BSL).

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA14 | Standard Input with internal pull-up |
| GPIOB | PB20 | Standard Output |
| GPIOA | PA30 | Standard Output |
| SYSCTL |  |  |
| I2C0 | PA10 | I2C Serial Data line (SDA) |
| I2C0 | PA11 | I2C Serial Clock line (SCL) |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0C1106](https://www.ti.com/tool/LP-MSPM0C1106) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA14 | GPIOA | PA14 | J2_34 | PA14 is connected to S2 button and connect to GND when push the button |
| PB20 | GPIOB | PB20 | J1_29 | <ul><li>PB20 can be connected to LED2 Blue<br><ul><li>`J7 ON` Connect to LED2 Blue<br><li>`J7 OFF` Disconnect from LED2 Blue</ul></ul> |
| PA30 | GPIOA | PA30 | J1_10 | <ul><li>PA30 can be connected to LED2 Green<br><ul><li>`J9 ON` Connect to LED2 Green<br><li>`J9 OFF` Disconnect from LED2 Green</ul></ul> |
| PA10 | I2C0 | SDA | J2_15 | <ul><li>PA10 is 5V tolerant open-drain so it requires pull-up<br><ul><li>`J19 1:2` Use 3.3V pull-up<br><li>`J19 2:3` Use 5V pull-up</ul><br><li>PA0 can be connected to LED1<br><ul><li>`J4 OFF` Disconnect from LED1</ul></ul> |
| PA11 | I2C0 | SCL | J2_35 | <ul><li>PA11 is 5V tolerant open-drain so it requires pull-up</ul></ul> |
| PA20 | DEBUGSS | SWCLK | N/A | PA20 is used by SWD during debugging<br><ul><li>`J101 15:16 ON` Connect to XDS-110 SWCLK while debugging<br><li>`J101 15:16 OFF` Disconnect from XDS-110 SWCLK if using pin in application |
| PA19 | DEBUGSS | SWDIO | N/A | PA19 is used by SWD during debugging<br><ul><li>`J101 13:14 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J101 13:14 OFF` Disconnect from XDS-110 SWDIO if using pin in application |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0C1106 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#manual-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0C1106 User's Guide](https://www.ti.com/lit/slau950).

## Example Usage
This demo's firmware is used for the BSL host device. 

The example will begin toggling LED1 while waiting for the user to press
the S2 button or send 0x22 from host to trigger BSL.

When S2 is pressed or 0x22 received, the application will:
1. Clear Blue LED1
2. Blink Green LED2
3. Reset the device with BOOTLOADERENTRY reset level

This will run the bootcode, which will:
- Check if there is a BSL request from the application
- Invoke the BSL if the bootloader is enabled in nonmain
- At this point, both LED1 and LED2 will be off

Once the BSL has been invoked, the user can send commands to the BSL through the
external BSL Host.

To return to application code, the user can either:
- Trigger a POR/BOR on the device
- Use the BSL Host to send the application start command to the device
