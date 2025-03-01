## Example Summary

This demo shows how to program a MSPM0C1104 MCU using a LP-MSPM0C1104 as host by BSL I2C interface.

More information please refer to [MSPM0 Bootloader (BSL) Implementation](https://www.ti.com/lit/slaae88) or [MSPM0 Bootloader user's guide](https://www.ti.com/lit/slau887)

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA16 | Standard Input with internal pull-up |
| GPIOA | PA22 | Standard Output |
| GPIOA | PA2 | Standard Output(To control NRST pin, just hardware invoke needed) |
| GPIOA | PA4 | Standard Output(To control invoke pin,  just hardware invoke needed) |
| SYSCTL |  |  |
| I2C0 | PA0 | I2C Serial Data line (SDA) |
| I2C0 | PA11 | I2C Serial Clock line (SCL) |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0C1104](https://www.ti.com/tool/LP-MSPM0C1104) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA16 | GPIOA | PA16 | N/A | <ul><li>PA16 is connected to S2 button to GND with no external pull resistor<br><ul><li>Press `S2` button to connect pin to GND<br><li>Don't use `S2` button if not needed by application</ul></ul> |
| PA22 | GPIOA | PA22 | J7 | <ul><li>PA22 can be connected to LED1<br><ul><li>`J7 ON` Connect to LED1<br><li>`J7 OFF` Disconnect from LED1</ul></ul> |
| PA2 | GPIOA | PA2 |   N/A |  Just hardware invoke needed |
| PA4 | GPIOA | PA4 | N/A |  Just hardware invoke needed |
| PA0 | I2C0 | SDA |  N/A |
| PA11 | I2C0 | SCL | J8 | <ul><li>PA11 can be connected to Pull up resistor:<br><ul><li>`J8 `: Connect pin to pull up resistor</ul></ul> |
| PA20 | DEBUGSS | SWCLK | J2_13 | <ul><li>PA20 is used by SWD during debugging<br><ul><li>`J101 15:16 ON` Connect to XDS-110 SWCLK while debugging<br><li>`J101 15:16 OFF` Disconnect from XDS-110 SWCLK if using pin in application</ul></ul> |
| PA19 | DEBUGSS | SWDIO | J2_17 | <ul><li>PA19 is used by SWD during debugging<br><ul><li>`J101 13:14 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J101 13:14 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0C1104 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0C1104 User's Guide](https://www.ti.com/lit/slau908).

## Example Usage

Connect the hardware that descriped in the document. Compile, load and run the example.
Push the S2 button to start program MSPM0C1104.
Note: if use software trigger need the application code(include software invoke) exist on the chip. 
