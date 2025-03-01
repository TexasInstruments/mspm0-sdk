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

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOA | Standard Input with internal pull-up | PA14 | S2 |  |
| GPIOA | Standard Output | PA0 | Red LED1 | Populate Jumper(s): J4[1:2] |
| GPIOA | High-Speed Output | PB20 | J1_29 |  |
| SYSCTL |  |  |  |  |
| I2C0 | I2C Serial Data line (SDA) | PA30 | J1_10 |  |
| I2C0 | I2C Serial Clock line (SCL) | PA27 | J1_9 |  |
| EVENT |  |  |  |  |
| BOARD | Debug Clock | PA20 | J13_4 | Populate Component(s): J13 |
| BOARD | Debug Data In Out | PA19 | J13_2 | Populate Component(s): J13 |

### Device Migration Recommendations
This project was developed for a superset device included in the MSPM0 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the LP-MSPM0H3216 User's Guide.

## Example Usage

Connect SDA and SCL between I2C Controller and Target.
Note that I2C requires external pull-ups which can be connected externally or enabled
using the LaunchPad.
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
