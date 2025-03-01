## Example Summary

This example implements an I2C Controller using the MSP Communication modules - I2C. This example demonstrates sending commands to an I2C Target to read from or write to its memory address and shows the response.


## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| I2C0 | PA0 | I2C Serial Data line (SDA) |
| I2C0 | PA1 | I2C Serial Clock line (SCL) |
| CRC |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3519](https://www.ti.com/tool/LP-MSPM0G3519) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA0 | I2C0 | SDA | J2_2/J5_50  | <ul><li>PA0 can be connected to an on-board pull-up resistor<br><ul><li>`J2 1:2 ON` Connect to 3.3V Pullup<br></ul></ul> |
| PA1 | I2C0 | SCL | J3_2/J5_49 | <ul><li>PA1 can be connected to an on-board pull-up resistor<br><ul><li>`J3 1:2 ON` Connect to 3.3V Pullup<br></ul></ul> |
| PA20 | DEBUGSS | SWCLK | N/A | <ul><li>PA20 is used by SWD during debugging<br><ul><li>`J19 15:16 ON` Connect to XDS-110 SWCLK while debugging<br><li>`J19 15:16 OFF` Disconnect from XDS-110 SWCLK if using pin in application</ul></ul> |
| PA19 | DEBUGSS | SWDIO | N/A | <ul><li>PA19 is used by SWD during debugging<br><ul><li>`J19 13:14 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J19 13:14 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0G3519 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0G3519 User's Guide](https://www.ti.com/lit/slau947).

## Example Usage
Connect SDA and SCL of I2C Controller to SDA and SCL of I2C Target respectively. Compile, load and run the example.

Please refer to MSPM0 Communication Modules - I2C User Guide for more details.