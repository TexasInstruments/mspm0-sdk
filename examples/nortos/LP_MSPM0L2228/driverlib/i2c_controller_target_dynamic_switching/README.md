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
| GPIOA | PA23 | Standard Output |
| GPIOB | PB8 | Standard Input with internal pull-up |
| GPIOB | PB9 | Standard Output |
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
| PA23 | GPIOA | PA23 | N/A | <ul><li>PA23 can be connected to VCC using R8 or to GND using C9<br><ul><li>`R8` and `C9` are not soldered by default allowing the pin to be used for other functions</ul><br><li>PA23 can be connected to LED4 Blue<br><ul><li>`J4 ON` Connect to LED4 Blue<br><li>`J4 OFF` Disconnect from LED4 Blue</ul></ul> |
| PB8 | GPIOB | PB8 | J7_15 | <ul><li>PB8 is connected to S3 button to GND with no external pull resistor<br><ul><li>Press `S3` button to connect pin to GND<br><li>Don't use `S3` button if not needed by application</ul></ul> |
| PB9 | GPIOB | PB9 | N/A | <ul><li>PB9 can be connected to LED4 Green<br><ul><li>`J6 ON` Connect to LED4 Green<br><li>`J6 OFF` Disconnect from LED4 Green</ul></ul> |
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
