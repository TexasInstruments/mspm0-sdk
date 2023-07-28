## Example Summary

This demo shows how to program a MSPM0L1306 MCU using a LP-MSPM0G3507 as host by BSL I2C interface.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA0 | Open-Drain Output |
| GPIOB | PB21 | Standard Input with internal pull-up |
| GPIOB | PB0 | Standard Output |
| GPIOB | PB16 | Standard Output |
| GPIOB | PB27 | Standard Output |
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
| PA0 | GPIOA | PA0 | J27_9 | This pin is 5V tolerant open-drain and requires pull-up.<br>J4 ON/OFF: Connect/Disconnect LED1<br>J19 1:2 3.3V Pull-up<br>J19 2:3 5V Pull-up |
| PB21 | GPIOB | PB21 | J27_4 | Connected to S2 button (no pull resistor) |
| PB0 | GPIOB | PB0 | J2_12 | N/A |
| PB16 | GPIOB | PB16 | J2_11 | N/A |
| PB27 | GPIOB | PB27 | J27_10 | J7 ON/OFF Connect/Disconnect LED2 Green |
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

Connect the hardware that descriped in the document. Compile, load and run the example.
Push the S2 button to start program MSPM0L1306.
