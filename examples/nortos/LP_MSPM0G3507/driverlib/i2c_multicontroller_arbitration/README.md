## Example Summary

This example shows detection of arbitration error in a Multi-Controller bus.
I2C Controller writes multiple bytes to a target device until an arbitration error occurs.
LED toggles if transmission is successful, LED will remain ON after arbitration error is detected.
One or more additional I2C controllers are needed to cause an arbitration error.
The Target device must be enabled and active before running this example.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOB | PB26 | Standard Output |
| SYSCTL |  |  |
| I2C1 | PB3 | I2C Serial Data line (SDA) |
| I2C1 | PB2 | I2C Serial Clock line (SCL) |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PB26 | GPIOB | PB26 | J27_8 | J6 ON/OFF Connect/Disconnect LED2 Red |
| PB3 | I2C1 | SDA | J1_10 | R60 can be soldered for pull-up |
| PB2 | I2C1 | SCL | J1_9 | R59 can be soldered for pull-up |
| PA20 | DEBUGSS | SWCLK | N/A | J101 15:16 ON: Connect to XDS-110 SWCLK (debug) |
| PA19 | DEBUGSS | SWDIO | N/A | J101 13:14 ON: Connect to XDS-110 SWDIO (debug) |

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0G3507 User's Guide](https://www.ti.com/lit/slau846).

## Example Usage

Connect SDA and SCL between I2C Controllers and Target(s).
Compile, load and run the example.
LED2 will toggle if transmission is successful.
LED2 will remain on if an arbitration error is detected.
