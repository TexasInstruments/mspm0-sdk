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
| GPIOA | PA25 | Standard Input |
| GPIOA | PA9 | Standard Input |
| GPIOA | PA15 | Standard Input |
| GPIOA | PA8 | Standard Input |
| GPIOA | PA16 | Standard Input |
| GPIOA | PA24 | Standard Input |
| GPIOA | PA18 | Standard with Wake Input |
| GPIOA | PA26 | Standard Input |
| GPIOA | PA23 | Standard Output |
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
| PA25 | GPIOA | PA25 | J1_2 | <ul><li>PA25 can be connected to photodiode D1<br><ul><li>`J4 OFF` Disconnect from photodiode D1</ul></ul> |
| PA9 | GPIOA | PA9 | J1_3 | <ul><li>PA9 can be connected to XDS-110 backchannel or to boosterpack connector:<br><ul><li>To use on boosterpack connector and disconnect XDS-110:<br>  `J17 2:3`: Connect pin to `J1_3`</ul></ul> |
| PA15 | GPIOA | PA15 | J3_23 | <ul><li>PA15 can be connected to thermistor RT1<br><ul><li>`J1 2:3/OFF` Disconnect thermistor RT1</ul></ul> |
| PA8 | GPIOA | PA8 | J1_4 | <ul><li>PA8 can be connected to XDS-110 backchannel or to boosterpack connector:<br><ul><li>To use on boosterpack connector and disconnect XDS-110:<br>  `J16 2:3`: Connect pin to `J1_4`</ul></ul> |
| PA16 | GPIOA | PA16 | J3_24 | N/A |
| PA24 | GPIOA | PA24 | J1_5 | <ul><li>PA24 can be connected to photodiode D1<br><ul><li>`J5 OFF` Disconnect from photodiode D1</ul></ul> |
| PA18 | GPIOA | PA18 | J3_26 | <ul><li>PA18 can be connected to S1 button to VCC with external pull-down<br><ul><li>`J11 ON` Connect S1 button and external pull-down<br><li>`J11 OFF` Disconnect S1 button and external pull-down</ul><br><li>PA18 can be connected to XDS-110 BSL_Invoke<br><ul><li>`J101 17:18 OFF` Disconnect pin to XDS-110 BSL Invoke</ul></ul> |
| PA26 | GPIOA | PA26 | J4_38 | <ul><li>PA26 can be connected to LED2 Red<br><ul><li>`J12 ON` Connect to LED2 Red<br><li>`J12 OFF` Disconnect from LED2 Red</ul></ul> |
| PA23 | GPIOA | PA23 | J2_12 | N/A |
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

Compile, load and run the example. Connect the I2C SDA and SCL lines to the expander and ensure that the necessary Jumpers for pull-ups and/or LEDs are connected. Please refer to the related [app note](https://www.ti.com/lit/slaael4) for more details.
