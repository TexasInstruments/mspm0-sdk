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
| TIMG2 | PA3 | Counter Compare Pin 0 |
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
| PA14 | GPIOA | PA14 | N/A | Connected to S2 button (no pull resistor) |
| PA24 | GPIOA | PA24 | J1_5 | J5 OFF: Disconnect from photodiode D1<br>R93/R94 for thermistor RT1 not soldered by default |
| PA6 | GPIOA | PA6 | J1_7 | N/A |
| PA22 | GPIOA | PA22 | J1_8 | J6 OFF: Disconnect from photodiode D1<br>RC R95/C57 for thermistor RT1 not soldered by default |
| PA12 | GPIOA | PA12 | J4_40 | N/A |
| PA13 | GPIOA | PA13 | J4_39 | J3 ON/OFF: Connect/Disconnect LED2 Green |
| PA7 | GPIOA | PA7 | J2_18 | N/A |
| PA27 | GPIOA | PA27 | J4_37 | J13 ON/OFF: Connect/Disconnect LED2 Blue |
| PA26 | GPIOA | PA26 | J4_38 | J12 ON/OFF: Connect/Disconnect LED2 Red |
| PA4 | GPIOA | PA4 | J2_14 | N/A |
| PA5 | GPIOA | PA5 | J2_15 | N/A |
| PA3 | TIMG2 | CCP0 | J2_19 | N/A |
| PA10 | I2C0 | SDA | J4_36 | N/A |
| PA11 | I2C0 | SCL | J2_11/J4_35 | J14 1:2 Connect to J2_11<br>J14 2:3 Connect to J4_35 |
| PA9 | UART0 | RX | J1_3 | J17 1:2 Connect XDS-110 BackChannel UART on J101<br>J17 2:3 Connect to J1_3<br>J101 7:8 Connect XDS-110 BackChannel UART RX |
| PA8 | UART0 | TX | J1_4 | J16 1:2 Connect XDS-110 BackChannel UART on J101<br>J16 2:3 Connect to J1_4<br>J101 9:10 Connect XDS-110 BackChannel UART TX |
| PA25 | ADC0 | A2 | J1_2 | J4 ON/OFF: Connect/disconnect from photodiode D1<br>J1 2:3: Connect to Thermistor RT1<br>J15 ON: Connects Thermistor RT1, if needed |
| PA23 | VREF | VREF+ | J2_12 | C8/C11 to GND can be soldered if needed |
| PA21 | VREF | VREF- | J1_6 | Solder R4 to GND if needed |
| PA17 | OPA1 | IN1- | J3_25 | N/A |
| PA20 | DEBUGSS | SWCLK | J2_13 | J101 15:16 ON: Connect to XDS-110 SWCLK (debug) |
| PA19 | DEBUGSS | SWDIO | J2_17 | J101 13:14 ON: Connect to XDS-110 SWDIO (debug) |

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
