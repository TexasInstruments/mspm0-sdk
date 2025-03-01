## Example Summary

I2C Target responds to write and read request from I2C Controller.
The example uses FIFO and polling. It can only handle a maximum of 8 bytes due to FIFO size.
LED toggles if transmission and reception are successful.
The Target device must be enabled and active before running the Controller example.

This example can be used to validate I2C communication, as a starting point to
enable I2C functionality, or to create custom drivers.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOA | Standard Output | PA0 | Red LED1 | Populate Jumper(s): J4[1:2] |
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
Note that I2C requires pull-up resistors. Internal pull-ups can be enabled in
SysConfig (see datasheet for resistance specification), but external pull-ups
might be required based on I2C speed and capacitance. External pull-ups can be
connected or enabled using the LaunchPad.
Compile, load and run the example.
LED1 will toggle periodically if transmission and reception were successful.
LED1 will remain off if there is a problem during initialization.
LED1 will remain on if there is a problem during data transfers.
