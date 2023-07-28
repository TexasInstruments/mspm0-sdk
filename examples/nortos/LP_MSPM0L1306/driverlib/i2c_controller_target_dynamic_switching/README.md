## Example Summary

This example shows how the I2C module can dynamically switch between operating as
Controller and Target.

In I2C Controller mode, the device writes multiple bytes to a target device every 1s.
In I2C Target mode, the device responds to write requests from a controller device.
On pressing S2, the device can switch between operating in I2C Controller and Target modes.
The Blue LED toggles every 1s if the device is operating in I2C Controller mode, and transmission is successful.
The Green LED toggles every 1s if the device is operating in I2C Target mode, and reception is successful.
The example uses FIFO and interrupts.

For initializing the device in Controller mode, set the define I2C_CONTROLLER_MODE to 1.
For initializing the device in Target mode, set the define I2C_CONTROLLER_MODE to 0.

The Target device must be enabled and active before running this example.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA14 | Standard Input with internal pull-up |
| GPIOA | PA27 | Standard Output |
| GPIOA | PA13 | Standard Output |
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
| PA14 | GPIOA | PA14 | N/A | Connected to S2 button (no pull resistor) |
| PA27 | GPIOA | PA27 | J4_37 | J13 ON/OFF: Connect/Disconnect LED2 Blue |
| PA13 | GPIOA | PA13 | J4_39 | J3 ON/OFF: Connect/Disconnect LED2 Green |
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
Before compiling the example on the Target, make sure to set the define
I2C_CONTROLLER_MODE to 0, and then load and run the example.
Compile, load and run the example on the Controller device.
Make sure the Target device is running before starting the Controller.

In Controller mode, the Blue LED will toggle every 1s if transmission
was successful.
In Target mode, the Green LED will toggle every 1s if reception was
successful.

To switch between Controller and Target modes, press S2. Make sure to press
S2 on the Controller device first, since the Target device should always be
set up and running before the Controller.

LEDs will remain off if there is a problem during initialization.
LEDs will remain on if there is a problem during data transfers.
