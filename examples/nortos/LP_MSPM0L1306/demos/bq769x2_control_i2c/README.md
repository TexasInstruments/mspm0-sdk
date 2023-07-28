## Example Summary

This example shows the implementation of using MSPM0 to control BQ769x2 to detect the battery status. After pressing the button connected to PA14, MSPM0 will start communicaiting with BQ769x2 though the I2C port at PA0(SDA) and PA1(SCL).

The example uses a BQ769x2 which is a battery monitor, but the same framework can be adapted for other similar I2C-based battery management ICs from TI. Please refer to [BQ769x2 Control Based on MSPM0 Through I2C](https://www.ti.com/lit/slaaeb3) app note for more details.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA26 | Standard Output |
| GPIOA | PA14 | Standard Input with internal pull-up |
| SYSCTL |  |  |
| I2C0 | PA0 | I2C Serial Data line (SDA) |
| I2C0 | PA1 | I2C Serial Clock line (SCL) |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L1306](https://www.ti.com/tool/LP-MSPM0L1306) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA26 | GPIOA | PA26 | J4_38 | J12 ON/OFF: Connect/Disconnect LED2 Red |
| PA14 | GPIOA | PA14 | N/A | Connected to S2 button (no pull resistor) |
| PA0 | I2C0 | SDA | J1_10 | This pin is 5V tolerant open-drain and requires pull-up.<br>J2 OFF: Disconnect LED1<br>J10 1:2 5V Pull-up<br>J10 2:3 3.3V Pull-up |
| PA1 | I2C0 | SCL | J1_9 | This pin is 5V tolerant open-drain and requires pull-up.<br>J9 1:2 5V Pull-up<br>J9 2:3 3.3V Pull-up |
| PA20 | DEBUGSS | SWCLK | J2_13 | J101 15:16 ON: Connect to XDS-110 SWCLK (debug) |
| PA19 | DEBUGSS | SWDIO | J2_17 | J101 13:14 ON: Connect to XDS-110 SWDIO (debug) |

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0L1306 User's Guide](https://www.ti.com/lit/slau869).

## Example Usage

This example was developed and tested using a [BQ769x2EVM](https://www.ti.com/tool/BQ76952EVM). Please refer to [BQ769x2 Control Based on MSPM0 Through I2C](https://www.ti.com/lit/slaaeb3) app note for more details.
