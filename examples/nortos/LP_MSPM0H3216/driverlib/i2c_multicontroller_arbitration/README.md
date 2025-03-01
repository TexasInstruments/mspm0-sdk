## Example Summary

This example shows detection of arbitration error in a Multi-Controller bus.
I2C Controller writes multiple bytes to a target device until an arbitration error occurs.
LED toggles if transmission is successful, LED will remain ON after arbitration error is detected.
One or more additional I2C controllers are needed to cause an arbitration error.
The Target device must be enabled and active before running this example.

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

Connect SDA and SCL between I2C Controller(s) and Target(s).
Note that I2C requires pull-up resistors. Internal pull-ups can be enabled in
SysConfig (see datasheet for resistance specification), but external pull-ups
might be required based on I2C speed and capacitance. External pull-ups can be
connected or enabled using the LaunchPad.
Compile, load and run the example.
LED1 will toggle if transmission is successful.
LED1 will remain on if an arbitration error is detected.
