## Example Summary

This example shows how to configure an I2C expander. The required data packet requires a command byte followed by a data byte: [CMD][DATA].

The commands to control the expander include:
- 0x1: GPIO direction set, where a 1 represents an output and a 0 represents an input in the DATA byte.
- 0x2: GPIO output control, where a 1 represents output HIGH and a 0 represents output LOW in the DATA byte.

A read request to the IO expander returns a byte containing the current state of all GPIO pins, regardless of direction.

When an input pin changes on the IO expander, the IRQ_OUT pin is pulled LOW until a read request is received. This signal is used to alert the controller to request a read from the IO expander.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA2 | Standard Input |
| GPIOA | PA27 | Standard Input |
| GPIOA | PA17 | Standard Input |
| GPIOA | PA24 | Standard Input |
| GPIOA | PA4 | Standard Input |
| GPIOA | PA6 | Standard Input |
| GPIOA | PA16 | Standard Input |
| GPIOA | PA22 | Standard Input |
| GPIOA | PA23 | Standard Output |
| SYSCTL |  |  |
| I2C0 | PA0 | I2C Serial Data line (SDA) |
| I2C0 | PA11 | I2C Serial Clock line (SCL) |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0C1104](https://www.ti.com/tool/LP-MSPM0C1104) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA2 | GPIOA | PA2 | J2_13 | N/A |
| PA27 | GPIOA | PA27 | J1_4 | <ul><li>PA27 can be connected to XDS-110 backchannel<br><ul><li>`J101 7:8 OFF` Disconnect XDS-110 backchannel UART TX</ul></ul> |
| PA17 | GPIOA | PA17 | J2_18 | N/A |
| PA24 | GPIOA | PA24 | J1_5 | N/A |
| PA4 | GPIOA | PA4 | J2_14 | N/A |
| PA6 | GPIOA | PA6 | J1_7 | N/A |
| PA16 | GPIOA | PA16 | J2_19 | <ul><li>PA16 is connected to S2 button to GND with no external pull resistor<br><ul><li>Press `S2` button to connect pin to GND<br><li>Don't use `S2` button if not needed by application</ul></ul> |
| PA22 | GPIOA | PA22 | J1_8 | N/A |
| PA23 | GPIOA | PA23 | J2_12 | N/A |
| PA0 | I2C0 | SDA | J1_10 | <ul><li>PA0 is 5V tolerant open-drain so it requires pull-up<br><ul><li>`J20 1:2` Use 3.3V pull-up<br><li>`J20 2:3` Use 5V pull-up</ul></ul> |
| PA11 | I2C0 | SCL | J1_9 | <ul><li>PA11 can be connected to an external 3.3V pull-up<br><ul><li>`J6 OFF` Disconnect 3.3V pull-up<br><li>`J6 ON` Connect 3.3V pull-up</ul></ul> |
| PA20 | DEBUGSS | SWCLK | J2_11 | <ul><li>PA20 is used by SWD during debugging<br><ul><li>`J101 13:14 ON` Connect to XDS-110 SWCLK while debugging<br><li>`J101 13:14 OFF` Disconnect from XDS-110 SWCLK if using pin in application</ul></ul> |
| PA19 | DEBUGSS | SWDIO | J2_17 | <ul><li>PA19 is used by SWD during debugging<br><ul><li>`J101 11:12 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J101 11:12 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0C1104 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0C1104 User's Guide](https://www.ti.com/lit/slau908).

## Example Usage

Compile, load and run the example. Connect the I2C SDA and SCL lines to the expander and ensure that the necessary Jumpers for pull-ups and/or LEDs are connected. Please refer to the related [app note](https://www.ti.com/lit/slaael4) for more details.
