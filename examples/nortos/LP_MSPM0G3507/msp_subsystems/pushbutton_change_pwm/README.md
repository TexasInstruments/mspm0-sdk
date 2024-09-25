## Example Summary

This example demonstrates using the two switches on the MSPM0G3507 Launchpad to change the period and duty cycle on TIMA1 PWM output channels CC0 and CC1. Pressing switch S1 changes the PWM period of the timer. Pressing S2 changes the duty cycle on CC1 output. This example also demonstrates the use of TIMA0 configured as a one-shot timer and interrupts for debouncing the switches.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA18 | Standard with Wake Input |
| GPIOB | PB22 | Standard Output |
| GPIOB | PB27 | Standard Output |
| GPIOB | PB26 | Standard Output |
| GPIOB | PB21 | Standard Input with internal pull-up |
| SYSCTL |  |  |
| TIMA1 | PA10 | Capture/Compare Pin 0 |
| TIMA1 | PA11 | Capture/Compare Pin 1 |
| TIMA0 |  |  |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA18 | GPIOA | PA18 | J3_26/J3_29 | <ul><li>PA18 can be connected to S1 button to VCC with external pull-down<br><ul><li>`J8 ON` Connect S1 button and external pull-down<br><li>`J8 OFF` Disconnect S1 button and external pull-down</ul><br><li>PA18 can be connected to XDS-110 BSL_Invoke<br><ul><li>`J101 17:18 OFF` Disconnect pin to XDS-110 BSL Invoke</ul><br><li>PA18 can be connected to boosterpack pin J3_29<br><ul><li>`J15 2:3` Connect pin to J3_29<br><li>`J15 1:2/OFF` Disconnect pin from J3_29</ul><br><li>PA18 can be connected to boosterpack pin J3_26<br><ul><li>`R58` is soldered and connects pin to J3_26 by default</ul></ul> |
| PB22 | GPIOB | PB22 | J27_5 | <ul><li>PB22 can be connected to LED2 Blue<br><ul><li>`J5 ON` Connect to LED2 Blue<br><li>`J15 OFF` Disconnect from LED2 Blue</ul></ul> |
| PB27 | GPIOB | PB27 | J27_10 | <ul><li>PB27 can be connected to LED2 Green<br><ul><li>`J7 ON` Connect to LED2 Green<br><li>`J7 OFF` Disconnect from LED2 Green</ul></ul> |
| PB26 | GPIOB | PB26 | J27_8 | <ul><li>PB26 can be connected to LED2 Red<br><ul><li>`J6 ON` Connect to LED2 Red<br><li>`J6 OFF` Disconnect from LED2 Red</ul></ul> |
| PB21 | GPIOB | PB21 | J27_4 | <ul><li>PB21 is connected to S2 button to GND with no external pull resistor<br><ul><li>Press `S2` button to connect pin to GND<br><li>Don't use `S2` button if not needed by application</ul></ul> |
| PA10 | TIMA1 | CCP0 | J4_34/J26_5 | <ul><li>PA10 can be connected to XDS-110 backchannel, boosterpack connector or CAN/LIN connector:<br><ul><li>To use on boosterpack connector and disconnect XDS-110:<br>  `J21 2:3`: Connect pin to `J4_34`<br><li>To use on J26 CAN/LIN connector:<br>  `R62` is populated by default and connects pin to `J26_5`</ul></ul> |
| PA11 | TIMA1 | CCP1 | J4_33/J26_6 | <ul><li>PA11 can be connected to XDS-110 backchannel, boosterpack connector or CAN/LIN connector:<br><ul><li>To use on boosterpack connector and disconnect XDS-110:<br>  `J22 2:3`: Connect pin to `J4_33`<br><li>To use on J26 CAN/LIN connector:<br>  `R63` is populated by default and connects pin to `J26_6`</ul></ul> |
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
Compile, load, and run the example. At startup, TIMA1 is loaded with a PWM period of 1ms (1000Hz) and 75% duty cycle on CC0 and 25% duty cycle on CC1.  Pressing switch S1 will cycle through the following PWM periods: 1ms, 2ms, 4ms, and 0.5ms.    These changes are reflected on both CC0 and CC1 outputs. Pressing switch S2 will cycle through the following PWM duty cycles: 75%, 90%, 25%, and 50%

