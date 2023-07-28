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
| SYSCTL |  |  |
| UART0 | PA9 | RX Pin |
| UART0 | PA8 | TX Pin |
| UART0 | PA13 | RTS Pin |
| UART0 | PA12 | CTS Pin |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L1306](https://www.ti.com/tool/LP-MSPM0L1306) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA9 | UART0 | RX | J1_3 | J17 1:2 Connect XDS-110 BackChannel UART on J101<br>J17 2:3 Connect to J1_3<br>J101 7:8 Connect XDS-110 BackChannel UART RX |
| PA8 | UART0 | TX | J1_4 | J16 1:2 Connect XDS-110 BackChannel UART on J101<br>J16 2:3 Connect to J1_4<br>J101 9:10 Connect XDS-110 BackChannel UART TX |
| PA13 | UART0 | RTS | J4_39 | J3 OFF: Disconnect LED2 Green |
| PA12 | UART0 | CTS | J4_40 | N/A |
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
Connect this transmitter device to a receiver as follows:
- UART Transmitter TX  -> UART Receiver RX
- UART Transmitter CTS <- UART Receiver RTS

Compile, load and run the example.
Optionally, connect a logic analyzer or oscilloscope to observe hardware flow control.
Ensure that the UART Receiver example is running before starting this transmitter example.
Once the example is started, the UART Transmitter will start to transmit data.
