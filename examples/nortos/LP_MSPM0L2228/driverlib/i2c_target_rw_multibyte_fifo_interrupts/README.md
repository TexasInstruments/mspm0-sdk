## Example Summary

I2C Target responds to write and read request from I2C Controller.
The example uses FIFO and interrupts.
LED toggles if transmission and reception are successful.
The Target device must be enabled and active before running the Controller example.
This example can be used to validate I2C communication, as a starting point to
enable I2C functionality, or to create custom drivers.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOB | PB10 | Standard Output |
| SYSCTL |  |  |
| I2C0 | PA0 | I2C Serial Data line (SDA) |
| I2C0 | PA1 | I2C Serial Clock line (SCL) |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L2228](https://www.ti.com/tool/LP-MSPM0L2228) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PB10 | GPIOB | PB10 | N/A | <ul><li>PB10 can be connected to LED4 Red<br><ul><li>`J5 ON` Connect to LED4 Red<br><li>`J5 OFF` Disconnect from LED4 Red</ul></ul> |
| PA0 | I2C0 | SDA | J3_10 | <ul><li>PA0 is 5V tolerant open-drain so it requires pull-up<br><ul><li>`J12 1:2` Use 3.3V pull-up<br><li>`J12 2:3` Use 5V pull-up</ul><br><li>PA0 can be connected to LED3<br><ul><li>`J2 OFF` Disconnect from LED3</ul></ul> |
| PA1 | I2C0 | SCL | J3_9 | <ul><li>PA1 is 5V tolerant open-drain so it requires pull-up<br><ul><li>`J13 1:2` Use 3.3V pull-up<br><li>`J13 2:3` Use 5V pull-up</ul></ul> |
| PA20 | DEBUGSS | SWCLK | N/A | <ul><li>PA20 is used by SWD during debugging<br><ul><li>`J14 15:16 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J14 15:16 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |
| PA19 | DEBUGSS | SWDIO | N/A | <ul><li>PA19 is used by SWD during debugging<br><ul><li>`J14 13:14 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J14 13:14 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0L2228 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0L2228 web page](https://www.ti.com/tool/LP-MSPM0L2228).

## Example Usage

Connect SDA and SCL between I2C Controller and Target.
Compile, load and run the example.
LED2.Red will toggle if transmission and/or reception are successful.
LED2.Red will remain off if there is a problem during initialization.
LED2.Red will remain on if there is a problem during data transfers.
