## Example Summary

This example sets up the UART to wait to receive data from another UART device using DMA.
The example can be used with the uart_tx_multibyte_fifo_dma_interrupts example running on another device.
The DMA in configured in single transfer mode, using the static UART RX DMA event as the DMA trigger.
When data is received in the UART RXFIFO, this triggers the DMA to transfer the data from the UART RXFIFO to a buffer.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| UART0 | PA9 | RX Pin |
| UART0 | PA8 | TX Pin |
| EVENT |  |  |
| DMA |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L1306](https://www.ti.com/tool/LP-MSPM0L1306) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA9 | UART0 | RX | J1_3 | J17 1:2 Connect XDS-110 BackChannel UART on J101<br>J17 2:3 Connect to J1_3<br>J101 7:8 Connect XDS-110 BackChannel UART RX |
| PA8 | UART0 | TX | J1_4 | J16 1:2 Connect XDS-110 BackChannel UART on J101<br>J16 2:3 Connect to J1_4<br>J101 9:10 Connect XDS-110 BackChannel UART TX |
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
Compile, load and run the example.
Connect the RX and TX between two UART devices or to the Launchpad's back-channel UART.
The example will wait to receive data from the other device, and then triggers a breakpoint after 4 bytes are received.
The received data can be verified in the reception buffer.
