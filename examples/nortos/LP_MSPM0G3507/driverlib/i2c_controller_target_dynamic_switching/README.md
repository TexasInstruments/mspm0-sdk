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
| GPIOB | PB21 | Standard Input with internal pull-up |
| GPIOB | PB22 | Standard Output |
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
| PB21 | GPIOB | PB21 | J27_4 | Connected to S2 button (no pull resistor) |
| PB22 | GPIOB | PB22 | J27_5 | J5 ON/OFF Connect/Disconnect LED2 Blue |
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
