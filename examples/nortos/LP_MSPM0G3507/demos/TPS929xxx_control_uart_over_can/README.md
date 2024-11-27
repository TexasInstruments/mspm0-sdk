## Example Summary

This example shows the implementation of using MSPM0 to control TPS929240 to control multi-channel LEDs. After pressing the button connected to PB21 S2 on Launchpad, MSPM0 will start to control TPS929240 to change different patterns.

The example uses a TPS929240 which is a 24-channel, 40V high-side LED driver that controls the 8-bit output current and 12-bit PWM duty cycles. Please refer to [TPS929xxx LED Driver Control Using MSPM0 Through UART Over Can](https://www.ti.com/lit/pdf/slaael6) app note for more details.

## Peripherals & Pin Assignments

| Peripheral | Pin  | Function                                       |
| ---------- | ---- | ---------------------------------------------- |
| GPIOA      | PA0  | Open-Drain Output                              |
| GPIOA      | PA18 | Standard with Wake Input with internal pull-up |
| GPIOB      | PB27 | Standard Output                                |
| GPIOB      | PB21 | Standard Input with internal pull-up           |
| SYSCTL     |   N/A   | N/A                                               |
| TIMG7      | PB15 | Capture/Compare Pin 0                          |
| TIMG0      |   N/A   | Delay Timer                        |
| TIMG6      |   N/A   | Timeout Timer                             |
| UART0      | PA11 | RX Pin                                         |
| UART0      | PA10 | TX Pin                                         |
| UART1      | PA9  | RX Pin                                         |
| UART1      | PA8  | TX Pin                                         |
| EVENT      |   N/A   |  N/A                           |
| DMA        |   N/A   | UART1 send and receive DMA with TPS929240   |
| DEBUGSS    | PA20 | Debug Clock                                    |
| DEBUGSS    | PA19 | Debug Data In Out                              |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA0 | GPIOA | PA0 | J27_9 | PA0 is connected to LED1. <br><ul><li>PA0 is 5V tolerant open-drain so it requires pull-up<br><ul><li>`J19 1:2` Use 3.3V pull-up<br><li>`J19 2:3` Use 5V pull-up</ul><br><li>PA0 can be connected to LED1<br><ul><li>`J4 ON` Connect to LED1<br><li>`J4 OFF` Disconnect from LED1</ul></ul> |
| PA18 | GPIOA | PA18 | J3_26/J3_29 | PA18 is connected to S1 button for EEPROM function. <br><ul><li>PA18 can be connected to S1 button to VCC with external pull-down<br><ul><li>`J8 ON` Connect S1 button and external pull-down<br><li>`J8 OFF` Disconnect S1 button and external pull-down</ul><br><li>PA18 can be connected to XDS-110 BSL_Invoke<br><ul><li>`J101 17:18 OFF` Disconnect pin to XDS-110 BSL Invoke</ul><br><li>PA18 can be connected to boosterpack pin J3_29<br><ul><li>`J15 2:3` Connect pin to J3_29<br><li>`J15 1:2/OFF` Disconnect pin from J3_29</ul><br><li>PA18 can be connected to boosterpack pin J3_26<br><ul><li>`R58` is soldered and connects pin to J3_26 by default</ul></ul> |
| PB27 | GPIOB | PB27 | J27_10 | PB27 is connected to LED2 Green. <br><ul><li>PB27 can be connected to LED2 Green<br><ul><li>`J7 ON` Connect to LED2 Green<br><li>`J7 OFF` Disconnect from LED2 Green</ul></ul> |
| PB21 | GPIOB | PB21 | J27_4 | PA21 is connected to S2 button to control the LED patterns. <br><ul><li>PB21 is connected to S2 button to GND with no external pull resistor<br><ul><li>Press `S2` button to connect pin to GND<br><li>Don't use `S2` button if not needed by application</ul></ul> |
| PB15 | TIMG7 | CCP0 | J2_17 | N/A now. |
| PA11 | UART0 | RX | J4_33/J26_6 | UART0 can be used as a debug port. <br><ul><li>PA11 can be used as UART RX connected to XDS-110 backchannel, to boosterpack connector or to CAN/LIN connector:<br><ul><li>To use backchannel UART on J101:<br>  `J22 1:2`: Connects XDS-110 backchannel UART RX to `J101`<br>  `J101 7:8` Connects XDS-110 backchannel to UART RX<br><li>To use UART on boosterpack connector:<br>  `J22 2:3`: Connects UART RX to `J4_33`<br><li>To use on J26 CAN/LIN connector:<br>  `R63` is populated by default and connects pin to `J26_6`</ul></ul> |
| PA10 | UART0 | TX | J4_34/J26_5 | UART0 can be used as a debug port. <br><ul><li>PA10 can be used as UART TX connected to XDS-110 backchannel, to boosterpack connector or to CAN/LIN connector:<br><ul><li>To use backchannel UART on J101:<br>  `J21 1:2`: Connects XDS-110 backchannel UART TX to `J101`<br>  `J101 9:10` Connects XDS-110 backchannel to UART TX<br><li>To use UART on boosterpack connector:<br>  `J21 2:3`: Connects UART TX to `J4_34`<br><li>To use on J26 CAN/LIN connector:<br>  `R62` is populated by default and connects pin to `J26_5`</ul></ul> |
| PA9 | UART1 | RX | J1_3 | UART1 RX is connected to TPS929240's UART TX pin. <br><ul><li>PA9 can be connected to J1_3<br><ul><li>`J1 2:3` Connect pin to J1_3<br><li>`J1 1:2/OFF` Disconnect pin from J1_3</ul> |
| PA8 | UART1 | TX | J1_4 | UART1 TX is connected to TPS929240's UART RX pin. <br> |
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

Compile, load and run the example.

Please refer to [TPS929xxx LED Driver Control Using MSPM0 Through UART Over Can](https://www.ti.com/lit/pdf/slaael6) app note for more details.
