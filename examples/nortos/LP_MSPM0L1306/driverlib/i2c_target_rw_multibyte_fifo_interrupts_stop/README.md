## Example Summary
I2C Target responds to write and read request from I2C Controller while in STOP mode.
The device will wake up on address match.
The example uses FIFO and interrupts.
LED toggles if transmission and reception are successful.
The Target device must be enabled and active before running the Controller example.
This example can be used to validate I2C communication, as a starting point to enable I2C functionality, or to create custom drivers.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA26 | Standard Output |
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

Connect SDA and SCL between I2C Controller and Target.
Compile, load and run the example.
LED2.Red will toggle if transmission and/or reception are successful.
LED2.Red will remain off if there is a problem during initialization.
LED2.Red will remain on if there is a problem during data transfers.

## Important Note
Early experimental samples have a known issue where the user is unable to
reconnect their debugger connection after the device has gone into STOP or
STANDBY mode. Please review advisory DEBUGSS_02 to see the steps to regain
the ability to connect the device to the debugger. This information is located
in the "MSPM0G and MSPM0L Families: Functional Differences With Early Samples"
document.
