## Example Summary

I2C Controller writes and reads multiple bytes to/from a target device.
The example uses FIFO and polling. It can only handle a maximum of 8 bytes due to FIFO size.
LED toggles if transmission and reception are successful.
The Target device must be enabled and active before running this example.
This example can be used to validate I2C communication, as a starting point to
control any I2C target, or to create custom drivers.
## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA26 | Standard Output |
| GPIOA | PA6 | Standard Output |
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
| PA26 | GPIOA | PA26 | J4_38 | <ul><li>PA26 can be connected to LED2 Red<br><ul><li>`J12 ON` Connect to LED2 Red<br><li>`J12 OFF` Disconnect from LED2 Red</ul></ul> |
| PA6 | GPIOA | PA6 | J1_7 | N/A |
| PA0 | I2C0 | SDA | J1_10 | <ul><li>PA0 is 5V tolerant open-drain so it requires pull-up<br><ul><li>`J10 2:3` Use 3.3V pull-up<br><li>`J10 1:2` Use 5V pull-up</ul><br><li>PA0 can be connected to LED1<br><ul><li>`J2 OFF` Disconnect from LED1</ul></ul> |
| PA1 | I2C0 | SCL | J1_9 | <ul><li>PA1 is 5V tolerant open-drain so it requires pull-up<br><ul><li>`J19 2:3` Use 3.3V pull-up<br><li>`J9 1:2` Use 5V pull-up</ul></ul> |
| PA20 | DEBUGSS | SWCLK | J2_13 | <ul><li>PA20 is used by SWD during debugging<br><ul><li>`J101 15:16 ON` Connect to XDS-110 SWCLK while debugging<br><li>`J101 15:16 OFF` Disconnect from XDS-110 SWCLK if using pin in application</ul></ul> |
| PA19 | DEBUGSS | SWDIO | J2_17 | <ul><li>PA19 is used by SWD during debugging<br><ul><li>`J101 13:14 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J101 13:14 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0L1306 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

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
LED2.Red will toggle periodically if transmission and reception were successful.
LED2.Red will remain off if there is a problem during initialization.
LED2.Red will remain on if there is a problem during data transfers.
USER_TEST_PIN GPIO will mimic the behavior of the LED pin on the BoosterPack
header and can be used to verify the LED behavior.
