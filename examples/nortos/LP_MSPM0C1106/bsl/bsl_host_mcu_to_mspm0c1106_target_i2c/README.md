## Example Summary

This demo shows how to program a MSPM0C1106 MCU using a LP-MSPM0C1106 as host by BSL I2C interface.

More information please refer to [MSPM0 Bootloader (BSL) Implementation](https://www.ti.com/lit/slaae88) or [MSPM0 Bootloader user's guide](https://www.ti.com/lit/slau887)

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA14 | Standard Input with internal pull-up |
| GPIOA | PA0 | Standard Output |
| GPIOA | PA13 | Standard Output(To control NRST pin of target, just needed for hardware invoke mode) |
| GPIOA | PA12 | Standard Output(To control BSL Invoke pin of target, just needed hardware invoke mode) |
| SYSCTL |  |  |
| I2C0 | PB3 | I2C Serial Data line (SDA) |
| I2C0 | PB2 | I2C Serial Clock line (SCL) |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0C1106](https://www.ti.com/tool/LP-MSPM0C1106) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA14 | GPIOA | PA14 | J2_19 | PA14 is connected to S2 button and connect to GND when push the button|
| PA13 | GPIOA | PA13 | J2_18 | Standard Output(To control NRST pin, just hardware invoke needed) |
| PA12 | GPIOA | PA12 | J2_17 | Standard Output(To control invoke pin,  just hardware invoke needed) |
| PA0 | GPIOA | PA0 | J2_34 |  PA0 can be connected to LED1<br><ul><li>`J4 ON` Connect to LED1 |
| PB3 | I2C0 | SDA | J2_32 | PB3 requires pull-up external |
| PB2 | I2C0 | SCL | J2_31 | PB2 requires pull-up external |
| PA20 | DEBUGSS | SWCLK | N/A | <ul><li>PA20 is used by SWD during debugging<br><ul><li>`J101 15:16 ON` Connect to XDS-110 SWCLK while debugging<br><li>`J101 15:16 OFF` Disconnect from XDS-110 SWCLK if using pin in application</ul></ul> |
| PA19 | DEBUGSS | SWDIO | N/A | <ul><li>PA19 is used by SWD during debugging<br><ul><li>`J101 13:14 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J101 13:14 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0C1106 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0C1106 User's Guide](https://www.ti.com/lit/slau950).

## Example Usage

Connect the hardware as described in the document. Compile, load and run the example.
Push the S2 button to start BSL to program MSPM0C1106.

> **Note:** To use software trigger mode, you need to ensure that the application code (`bsl_software_invoke_app_demo`) exists on the target and the global define of `Software_Invoke` is enabled in the project.
