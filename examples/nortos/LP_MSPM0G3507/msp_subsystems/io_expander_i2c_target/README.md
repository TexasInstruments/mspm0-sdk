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
| GPIOA | PA8 | Standard Input |
| GPIOA | PA22 | Standard Input |
| GPIOA | PA26 | Standard Input |
| GPIOA | PA24 | Standard Input |
| GPIOA | PA27 | Standard Input |
| GPIOA | PA18 | Standard with Wake Input |
| GPIOA | PA0 | Open-Drain Input |
| GPIOA | PA12 | High-Speed Output |
| SYSCTL |  |  |
| I2C0 | PA10 | I2C Serial Data line (SDA) |
| I2C0 | PA11 | I2C Serial Clock line (SCL) |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA25 | GPIOA | PA25 | J1_2 | N/A |
| PA8 | GPIOA | PA8 | J1_4 | N/A |
| PA22 | GPIOA | PA22 | J3_24 | <ul><li>PA22 can be connected to photodiode D1<br><ul><li>`J16 OFF` Disconnect from photodiode D1</ul>></ul> |
| PA26 | GPIOA | PA26 | J1_5 | <ul><li>PA26 can be connected to photodiode D1<br><ul><li>`J18 OFF` Disconnect from photodiode D1</ul><br><li>PA26 can be connected to thermistor RT1<br><ul><li>`J9 1:2/OFF` Disconnect from thermistor RT1</ul></ul> |
| PA24 | GPIOA | PA24 | J3_27 | N/A |
| PA27 | GPIOA | PA27 | J1_8 | <ul><li>PA27 can be connected to photodiode D1<br><ul><li>`J17 OFF` Disconnect from photodiode D1</ul></ul> |
| PA18 | GPIOA | PA18 | J3_26/J3_29 | <ul><li>PA18 can be connected to S1 button to VCC with external pull-down<br><ul><li>`J8 ON` Connect S1 button and external pull-down<br><li>`J8 OFF` Disconnect S1 button and external pull-down</ul><br><li>PA18 can be connected to XDS-110 BSL_Invoke<br><ul><li>`J101 17:18 OFF` Disconnect pin to XDS-110 BSL Invoke</ul><br><li>PA18 can be connected to boosterpack pin J3_29<br><ul><li>`J15 2:3` Connect pin to J3_29<br><li>`J15 1:2/OFF` Disconnect pin from J3_29</ul><br><li>PA18 can be connected to boosterpack pin J3_26<br><ul><li>`R58` is soldered and connects pin to J3_26 by default</ul></ul> |
| PA0 | GPIOA | PA0 | J27_9 | <ul><li>PA0 is 5V tolerant open-drain so it requires pull-up<br><ul><li>`J19 1:2` Use 3.3V pull-up<br><li>`J19 2:3` Use 5V pull-up</ul><br><li>PA0 can be connected to LED1<br><ul><li>`J4 ON` Connect to LED1<br><li>`J4 OFF` Disconnect from LED1</ul></ul> |
| PA12 | GPIOA | PA12 | J4_32/J26_1 | <ul><li>PA12 can be connected to CAN/LIN connector in addition to boosterpack connector:<br><ul><li>To use on J26 CAN/LIN connector:<br>  `R64` is populated by default and connects pin to `J26_1`</ul></ul> |
| PA10 | I2C0 | SDA | J4_34/J26_5 | <ul><li>PA10 can be connected to XDS-110 backchannel, boosterpack connector or CAN/LIN connector:<br><ul><li>To use on boosterpack connector and disconnect XDS-110:<br>  `J21 2:3`: Connect pin to `J4_34`<br><li>To use on J26 CAN/LIN connector:<br>  `R62` is populated by default and connects pin to `J26_5`</ul></ul> |
| PA11 | I2C0 | SCL | J4_33/J26_6 | <ul><li>PA11 can be connected to XDS-110 backchannel, boosterpack connector or CAN/LIN connector:<br><ul><li>To use on boosterpack connector and disconnect XDS-110:<br>  `J22 2:3`: Connect pin to `J4_33`<br><li>To use on J26 CAN/LIN connector:<br>  `R63` is populated by default and connects pin to `J26_6`</ul></ul> |
| PA20 | DEBUGSS | SWCLK | N/A | <ul><li>PA20 is used by SWD during debugging<br><ul><li>`J101 15:16 ON` Connect to XDS-110 SWCLK while debugging<br><li>`J101 15:16 OFF` Disconnect from XDS-110 SWCLK if using pin in application</ul></ul> |
| PA19 | DEBUGSS | SWDIO | N/A | <ul><li>PA19 is used by SWD during debugging<br><ul><li>`J101 13:14 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J101 13:14 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0G3507 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0G3507 User's Guide](https://www.ti.com/lit/slau873).

## Example Usage

Compile, load and run the example. Connect the I2C SDA and SCL lines to the expander and ensure that the necessary Jumpers for pull-ups and/or LEDs are connected. Please refer to the related [app note](https://www.ti.com/lit/slaael4) for more details.
