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
| UART1 | PA9 | RX Pin |
| UART1 | PA8 | TX Pin |
| UART1 | PB9 | RTS Pin |
| UART1 | PB8 | CTS Pin |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA9 | UART1 | RX | J1_3 | J1 2:3 Connect to J1_3 |
| PA8 | UART1 | TX | J1_4 | N/A |
| PB9 | UART1 | RTS | J1_7 | N/A |
| PB8 | UART1 | CTS | J2_15 | N/A |
| PA20 | DEBUGSS | SWCLK | N/A | J101 15:16 ON: Connect to XDS-110 SWCLK (debug) |
| PA19 | DEBUGSS | SWDIO | N/A | J101 13:14 ON: Connect to XDS-110 SWDIO (debug) |

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0G3507 User's Guide](https://www.ti.com/lit/slau846).

## Example Usage
Connect this transmitter device to a receiver as follows:
- UART Transmitter TX  -> UART Receiver RX
- UART Transmitter CTS <- UART Receiver RTS

Compile, load and run the example.
Optionally, connect a logic analyzer or oscilloscope to observe hardware flow control.
Ensure that the UART Receiver example is running before starting this transmitter example.
Once the example is started, the UART Transmitter will start to transmit data.
