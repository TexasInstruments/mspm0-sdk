## Example Summary

This example shows the implementation of using MSPM0 to control BQ76972 for reference design TIDA-010247. The example uses a BQ76972 which is a battery monitor, but the same framework can be adapted for other similar I2C-based battery management ICs from TI.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| I2C0 | PA10 | I2C Serial Data line (SDA) |
| I2C0 | PA11 | I2C Serial Clock line (SCL) |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [TIDA-010247](https://www.ti.com/tool/TIDA-010247) for reference design information, including user guide and hardware files.

| Pin | Peripheral | Function | TIDA Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA10 | I2C0 | SDA | J3_1 | <ul><li>Connect J3 pin1 and pin2 to enable SDA</ul></ul> |
| PA11 | I2C0 | SCL | J3_3 | <ul><li>Connect J3 pin3 and pin4 to enable SCL</ul></ul> |
| PA20 | DEBUGSS | SWCLK | J12-4 | <ul><li>PA20 is used by SWD during debugging<br><ul><li>`J101 15:16 ON` Connect to XDS-110 SWCLK while debugging<br><li>`J101 15:16 OFF` Disconnect from XDS-110 SWCLK if using pin in application</ul></ul> |
| PA19 | DEBUGSS | SWDIO | J12-2 | <ul><li>PA19 is used by SWD during debugging<br><ul><li>`J101 13:14 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J101 13:14 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |

### Device Migration Recommendations
This project was developed for a MSPM0G3519. Please visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration) for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.


## Example Usage

This example was developed and tested using a [TIDA-010247](https://www.ti.com/tool/TIDA-010247). Please refer to [TIDA-010247 Design Guide](https://www.ti.com/lit/tiduf20)for more details.
