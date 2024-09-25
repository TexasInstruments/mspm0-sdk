## Example Summary

The demo application provides a low power, high-performance, and very cost competitive motion detection solution using an
analog passive infrared (PIR) motion sensor and the MSPM0L1306. The MSPM0Lxx have integrated and configurable
operational amplifiers (OPAs) and digital to analog convertors (DACs). These OPAs can provide the PIR sensor’s entire signal chain, integrated inside the MSPM0. The OPA
are software configurable, allowing the signal chain to be easily adjusted for higher sensitivity and range, digital
feedback loops, and optimized for low power performance.

The signal chain design requires two DAC reference voltages, where the MSPM0Lxx only has one.  So this demo also implements a PWM DAC solution where it runs a PWM signal
through an active low-pass filter using the integrated GPAMP.

There is an assocaited application note for this demo, that explains the software solution further and lays out the hardware design this demo was developed for.


## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA0 | Open-Drain Output |
| GPIOA | PA4 | Standard Output |
| GPIOA | PA23 | Standard Output |
| SYSCTL |  |  |
| TIMG0 | PA12 | Counter Compare Pin 0 |
| TIMG1 |  |  |
| TIMG2 |  |  |
| I2C0 | PA10 | I2C Serial Data line (SDA) |
| I2C0 | PA11 | I2C Serial Clock line (SCL) |
| UART0 | PA9 | RX Pin |
| UART0 | PA8 | TX Pin |
| ADC0 | PA15 | ADC12 Channel 9 Pin |
| COMP0 |  |  |
| VREF |  |  |
| OPA1 | PA17 | OPA Inverting input 1 Pin |
| OPA1 | PA16 | OPA Output Pin |
| OPA0 | PA25 | OPA Non-inverting input 0 Pin |
| OPA0 | PA24 | OPA Inverting input 0 Pin |
| GPAMP | PA26 | GPAMP Non-inverting input Pin |
| GPAMP | PA22 | GPAMP Output Pin |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L1306](https://www.ti.com/tool/LP-MSPM0L1306) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA0 | GPIOA | PA0 | J1_10 | <ul><li>PA0 is 5V tolerant open-drain so it requires pull-up<br><ul><li>`J10 2:3` Use 3.3V pull-up<br><li>`J10 1:2` Use 5V pull-up</ul><br><li>PA0 can be connected to LED1<br><ul><li>`J2 ON` Connect to LED1<br><li>`J2 OFF` Disconnect from LED1</ul></ul> |
| PA4 | GPIOA | PA4 | J2_14 | N/A |
| PA23 | GPIOA | PA23 | J2_12 | N/A |
| PA12 | TIMG0 | CCP0 | J4_40 | N/A |
| PA10 | I2C0 | SDA | J4_36 | N/A |
| PA11 | I2C0 | SCL | J2_11/J4_35 | <ul><li>PA11 can be connected to two locations in boosterpack connector:<br><ul><li>`J14 1:2`: Connect pin to `J2_11`<br><li>`J14 2:3`: Connect pin to `J4_35`</ul></ul> |
| PA9 | UART0 | RX | J1_3 | <ul><li>PA9 can be used as UART RX connected to XDS-110 backchannel or to boosterpack connector:<br><ul><li>To use backchannel UART on J101:<br>  `J17 1:2`: Connects XDS-110 backchannel UART RX to `J101`<br>  `J101 7:8` Connects XDS-110 backchannel to UART RX<br><li>To use UART on boosterpack connector:<br>  `J17 2:3`: Connects UART RX to `J1_3`</ul></ul> |
| PA8 | UART0 | TX | J1_4 | <ul><li>PA8 can be used as UART TX connected to XDS-110 backchannel or to boosterpack connector:<br><ul><li>To use backchannel UART on J101:<br>  `J16 1:2`: Connects XDS-110 backchannel UART TX to `J101`<br>  `J101 9:10` Connects XDS-110 backchannel to UART TX<br><li>To use UART on boosterpack connector:<br>  `J16 2:3`: Connects UART TX to` J1_4`</ul></ul> |
| PA15 | ADC0 | A9 | J3_23 | <ul><li>PA15 can be connected to thermistor RT1<br><ul><li>`J1 2:3/OFF` Disconnect thermistor RT1<br><li>`J1 1:2` Connect thermistor RT1</ul><br><li>When using thermistor RT1:<br><ul><li>`J15 OFF` Disconnect power from thermistor RT1<br><li>`J15 ON` Connect power to thermistor RT1</ul></ul> |
| PA17 | OPA1 | IN1- | J3_25 | N/A |
| PA16 | OPA1 | OUT | J3_24 | N/A |
| PA25 | OPA0 | IN0- | J1_2 | <ul><li>PA25 can be connected to photodiode D1<br><ul><li>`J4 OFF` Disconnect from photodiode D1<br><li>`J4 ON` Connect to photodiode D1</ul><br><li>PA25 can be connected to thermistor RT1<br><ul><li>`J1 1:2/OFF` Disconnect from thermistor RT1<br><li>`J1 2:3` Connect to thermistor RT1</ul></ul> |
| PA24 | OPA0 | IN0- | J1_5 | <ul><li>PA24 can be connected to photodiode D1<br><ul><li>`J5 OFF` Disconnect from photodiode D1<br><li>`J5 ON` Cconnect to photodiode D1</ul><br><li>PA24 can be connected to thermistor RT1<br><ul><li>`R93/R94` circuit for thermistor RT1 is not soldered by default<br><li>Solder `R93/R94` RC circuit to connect to thermistor RT1</ul></ul> |
| PA26 | GPAMP | IN+ | J4_38 | <ul><li>PA26 can be connected to LED2 Red<br><ul><li>`J12 OFF` Disconnect from LED2 Red</ul></ul> |
| PA22 | GPAMP | OUT | J1_8 | <ul><li>PA22 can be connected to photodiode D1<br><ul><li>`J6 OFF` Disconnect from photodiode D1<br><li>`J6 ON` Connect to photodiode D1</ul></ul> |
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
