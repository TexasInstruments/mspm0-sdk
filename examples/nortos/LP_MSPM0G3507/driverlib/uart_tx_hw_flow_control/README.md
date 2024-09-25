## Example Summary

This example can be used with the uart_rx_hw_flow_control example running on another device.
This example sets up the UART with hardware flow control.

The application will begin by sending the first packet of data.
Since hardware flow control is enabled, the transmitter will automatically check that the CTS signal isasserted (low) before transmitting the next byte.
If CTS is de-asserted (high), the byte will not be transmitted out of the TX FIFO. Therefore we block if the TX FIFO is full before attempting to fill it.

After the first packet has been transmitted, the application will delay for some time to allow the UART Receiver application to re-configure its flow control settings.
After the delay, the application will begin to send the second packet of data.

After all data has been transmitted, the application will go to sleep.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA0 | Open-Drain Output |
| GPIOA | PA15 | Standard Output |
| SYSCTL |  |  |
| UART0 | PA11 | RX Pin |
| UART0 | PA10 | TX Pin |
| UART0 | PA8 | RTS Pin |
| UART0 | PB19 | CTS Pin |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA0 | GPIOA | PA0 | J27_9 | <ul><li>PA0 is 5V tolerant open-drain so it requires pull-up<br><ul><li>`J19 1:2` Use 3.3V pull-up<br><li>`J19 2:3` Use 5V pull-up</ul><br><li>PA0 can be connected to LED1<br><ul><li>`J4 ON` Connect to LED1<br><li>`J4 OFF` Disconnect from LED1</ul></ul> |
| PA15 | GPIOA | PA15 | J3_30 | <ul><li>This pin can be used for testing purposes in boosterpack connector<ul><li>Pin can be reconfigured for general purpose as necessary</ul></ul> |
| PA11 | UART0 | RX | J4_33/J26_6 | <ul><li>PA11 can be used as UART RX connected to XDS-110 backchannel, to boosterpack connector or to CAN/LIN connector:<br><ul><li>To use backchannel UART on J101:<br>  `J22 1:2`: Connects XDS-110 backchannel UART RX to `J101`<br>  `J101 7:8` Connects XDS-110 backchannel to UART RX<br><li>To use UART on boosterpack connector:<br>  `J22 2:3`: Connects UART RX to `J4_33`<br><li>To use on J26 CAN/LIN connector:<br>  `R63` is populated by default and connects pin to `J26_6`</ul></ul> |
| PA10 | UART0 | TX | J4_34/J26_5 | <ul><li>PA10 can be used as UART TX connected to XDS-110 backchannel, to boosterpack connector or to CAN/LIN connector:<br><ul><li>To use backchannel UART on J101:<br>  `J21 1:2`: Connects XDS-110 backchannel UART TX to `J101`<br>  `J101 9:10` Connects XDS-110 backchannel to UART TX<br><li>To use UART on boosterpack connector:<br>  `J21 2:3`: Connects UART TX to `J4_34`<br><li>To use on J26 CAN/LIN connector:<br>  `R62` is populated by default and connects pin to `J26_5`</ul></ul> |
| PA8 | UART0 | RTS | J1_4 | N/A |
| PB19 | UART0 | CTS | J3_23 | N/A |
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
Connect this transmitter device to a receiver as follows:
- UART Transmitter TX  -> UART Receiver RX
- UART Transmitter CTS <- UART Receiver RTS

Compile, load and run the example.
Optionally, connect a logic analyzer or oscilloscope to observe hardware flow control.
Ensure that the UART Receiver example is running before starting this transmitter example.
Once the example is started, the UART Transmitter will start to transmit data.
LED1 will turn on when the application completes. USER_TEST_PIN GPIO will mimic the
behavior of the LED pin on the BoosterPack header and can be used to verify the
LED behavior.
