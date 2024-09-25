## Example Summary

The demo application is of a Dual-Ray smoke detector, using the TPS880x AFE. This demo performs periodic measurements of the Dual-Ray AFE and implements a simple threshold algorithm to detect if an alarm should be triggered.  SysConfig is used to generate the ti_msp_dl_config.c and .h files, which fully configure all peripherals of the MSPM0.  The main clock is configured to run from the internal SYSOSC and is running at 32MHz.  No RTC is implemented in this example, instead a low power timer is configured in the TPS880x is used to wake the MSPM0 from sleep using an external GPIO.  These files also configure the GPIOS, Timers, Comms, and power policy for the application.

The main loop includes the sampling routine, averaging routines, determining if IR or blue LED thresholds have been reached, sending and receiving information from GUI if enabled, and entering low-power modes. The loop requires either IR or blue reflection to exceed the set threshold three times consecutively before sounding the alarm. As the warning level rises, the time between measurements is reduced.


## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA14 | Standard Input |
| GPIOA | PA24 | Standard Output |
| GPIOA | PA6 | Standard Output |
| GPIOA | PA22 | Standard Output |
| GPIOA | PA12 | Standard Output |
| GPIOA | PA13 | Standard Output |
| GPIOA | PA7 | Standard Input |
| GPIOA | PA27 | Standard Output |
| GPIOA | PA26 | Standard Output |
| GPIOA | PA4 | Standard Input |
| GPIOA | PA5 | High-Speed Input |
| SYSCTL |  |  |
| TIMG2 | PA3 | Capture/Compare Pin 0 |
| TIMG1 |  |  |
| TIMG0 |  |  |
| I2C0 | PA10 | I2C Serial Data line (SDA) |
| I2C0 | PA11 | I2C Serial Clock line (SCL) |
| UART0 | PA9 | RX Pin |
| UART0 | PA8 | TX Pin |
| ADC0 | PA25 | ADC12 Channel 2 Pin |
| COMP0 |  |  |
| VREF | PA23 | VREF Positive (VREF+) |
| VREF | PA21 | VREF Negative (VREF-) |
| OPA1 | PA17 | OPA Inverting input 1 Pin |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L1306](https://www.ti.com/tool/LP-MSPM0L1306) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA14 | GPIOA | PA14 | N/A | <ul><li>PA14 is connected to S2 button to GND with no external pull resistor<br><ul><li>Press `S2` button to connect pin to GND<br><li>Don't use `S2` button if not needed by application</ul></ul> |
| PA24 | GPIOA | PA24 | J1_5 | <ul><li>PA24 can be connected to photodiode D1<br><ul><li>`J5 OFF` Disconnect from photodiode D1</ul></ul> |
| PA6 | GPIOA | PA6 | J1_7 | N/A |
| PA22 | GPIOA | PA22 | J1_8 | <ul><li>PA22 can be connected to photodiode D1<br><ul><li>`J6 OFF` Disconnect from photodiode D1</ul></ul> |
| PA12 | GPIOA | PA12 | J4_40 | N/A |
| PA13 | GPIOA | PA13 | J4_39 | <ul><li>PA13 can be connected to LED2 Green<br><ul><li>`J3 ON` Connect to LED2 Green<br><li>`J3 OFF` Disconnect from LED2 Green</ul></ul> |
| PA7 | GPIOA | PA7 | J2_18 | N/A |
| PA27 | GPIOA | PA27 | J4_37 | <ul><li>PA27 can be connected to LED2 Blue<br><ul><li>`J13 ON` Connect to LED2 Blue<br><li>`J13 OFF` Disconnect from LED2 Blue</ul></ul> |
| PA26 | GPIOA | PA26 | J4_38 | <ul><li>PA26 can be connected to LED2 Red<br><ul><li>`J12 ON` Connect to LED2 Red<br><li>`J12 OFF` Disconnect from LED2 Red</ul></ul> |
| PA4 | GPIOA | PA4 | J2_14 | N/A |
| PA5 | GPIOA | PA5 | J2_15 | N/A |
| PA3 | TIMG2 | CCP0 | J2_19 | N/A |
| PA10 | I2C0 | SDA | J4_36 | N/A |
| PA11 | I2C0 | SCL | J2_11/J4_35 | <ul><li>PA11 can be connected to two locations in boosterpack connector:<br><ul><li>`J14 1:2`: Connect pin to `J2_11`<br><li>`J14 2:3`: Connect pin to `J4_35`</ul></ul> |
| PA9 | UART0 | RX | J1_3 | <ul><li>PA9 can be used as UART RX connected to XDS-110 backchannel or to boosterpack connector:<br><ul><li>To use backchannel UART on J101:<br>  `J17 1:2`: Connects XDS-110 backchannel UART RX to `J101`<br>  `J101 7:8` Connects XDS-110 backchannel to UART RX<br><li>To use UART on boosterpack connector:<br>  `J17 2:3`: Connects UART RX to `J1_3`</ul></ul> |
| PA8 | UART0 | TX | J1_4 | <ul><li>PA8 can be used as UART TX connected to XDS-110 backchannel or to boosterpack connector:<br><ul><li>To use backchannel UART on J101:<br>  `J16 1:2`: Connects XDS-110 backchannel UART TX to `J101`<br>  `J101 9:10` Connects XDS-110 backchannel to UART TX<br><li>To use UART on boosterpack connector:<br>  `J16 2:3`: Connects UART TX to` J1_4`</ul></ul> |
| PA25 | ADC0 | A2 | J1_2 | <ul><li>PA25 can be connected to photodiode D1<br><ul><li>`J4 OFF` Disconnect from photodiode D1</ul></ul> |
| PA23 | VREF | VREF+ | J2_12 | <ul><li>PA23 can be connected to GND using C8/C11 when used for VREF+<br><ul><li>`C8/C11` are not soldered by default<br><li>Solder `C8/C11` if needed for VREF+</ul></ul> |
| PA21 | VREF | VREF- | J1_6 | <ul><li>PA21 can be connected to GND using R4 when used for VREF-<br><ul><li>`R4` is not soldered by default<br><li>Solder `R4` to GND if nedded for VREF-</ul></ul> |
| PA17 | OPA1 | IN1- | J3_25 | N/A |
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
This demo software is provided in associatation with the [MSPM0 Lowpower dual-ray smoke detector application note.](https://www.ti.com/lit/slvafn0).  To run the demo, an external TPS8802 board and some jumper wire connections are needed.  Connect the launchpad to the board, program it, connect the EVMs to the Dual-ray smoke detector GUI for evaluation.

All of this and documentation of the software are included in the above app note.
