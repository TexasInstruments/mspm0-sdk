## Example Summary

This example demonstrates UART RX with RS485 mode. RS485 is usable with both UART Main and UART Extend.

This example can be used with the uart_rs485_send_packet example running on another device.

This example sets receive enable (RE) pin low, then receives a packet of data.
When the expected number of bytes have been received, LED1 will toggle.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA0 | Open-Drain Output |
| GPIOB | PB13 | Standard Output |
| GPIOB | PB3 | Standard Output |
| SYSCTL |  |  |
| UART0 | PA11 | RX Pin |
| UART0 | PA10 | TX Pin |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA0 | GPIOA | PA0 | J27_9 | <ul><li>PA0 is 5V tolerant open-drain so it requires pull-up<br><ul><li>`J19 1:2` Use 3.3V pull-up<br><li>`J19 2:3` Use 5V pull-up</ul><br><li>PA0 can be connected to LED1<br><ul><li>`J4 ON` Connect to LED1<br><li>`J4 OFF` Disconnect from LED1</ul></ul> |
| PB13 | GPIOB | PB13 | J4_35 | N/A |
| PB3 | GPIOB | PB3 | J1_10 | N/A |
| PA11 | UART0 | RX | J4_33/J26_6 | <ul><li>PA11 can be used as UART RX connected to XDS-110 backchannel, to boosterpack connector or to CAN/LIN connector:<br><ul><li>To use backchannel UART on J101:<br>  `J22 1:2`: Connects XDS-110 backchannel UART RX to `J101`<br>  `J101 7:8` Connects XDS-110 backchannel to UART RX<br><li>To use UART on boosterpack connector:<br>  `J22 2:3`: Connects UART RX to `J4_33`<br><li>To use on J26 CAN/LIN connector:<br>  `R63` is populated by default and connects pin to `J26_6`</ul></ul> |
| PA10 | UART0 | TX | J4_34/J26_5 | <ul><li>PA10 can be used as UART TX connected to XDS-110 backchannel, to boosterpack connector or to CAN/LIN connector:<br><ul><li>To use backchannel UART on J101:<br>  `J21 1:2`: Connects XDS-110 backchannel UART TX to `J101`<br>  `J101 9:10` Connects XDS-110 backchannel to UART TX<br><li>To use UART on boosterpack connector:<br>  `J21 2:3`: Connects UART TX to `J4_34`<br><li>To use on J26 CAN/LIN connector:<br>  `R62` is populated by default and connects pin to `J26_5`</ul></ul> |
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
Connect the device transmitting the RS485 packet to the receiving device using the following setup guidelines. You will need two RS485 transceiver boards, one for each MCU.
- Tranmistting MCU 3.3V         -> RS485 transceiver #1 3.3V
- Tranmistting MCU GND          -> RS485 transceiver #1 GND
- Tranmistting MCU TX (PA10)    -> RS485 transceiver #1 RX
- Tranmistting MCU RE (PB4)     -> RS485 transceiver #1 R/T
- Receiving MCU 3.3V            -> RS485 transceiver #2 3.3V
- Receiving MCU GND             -> RS485 transceiver #2 GND
- Receiving MCU RX (PA11)       -> RS485 transceiver #2 TX
- Receiving MCU DE (PB3)        -> RS485 transceiver #2 R/T
- (+) terminal of RS485 transceiver #1 -> (-) terminal of RS485 transceiver #2
- (-) terminal of RS485 transceiver #1 -> (+) terminal of RS485 transceiver #2

Compile, load and run the example. This example can be used in conjunction with the uart_rs485_send_packet example running on a separate device by first running uart_rs485_receive_packet on the device and then running uart_rs485_send_packet on the other device.
