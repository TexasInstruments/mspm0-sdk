## Example Summary

This example can be used with the uart_tx_hw_flow_control example running on another device.
This example sets up the UART with hardware flow control.

First, automatic hardware flow control is enabled, so the RTS flow control logic is linked to the RX FIFO threshold level.
The RTS is asserted (low) until the RX FIFO is filled up to the threshold level.
When the threshold level is reached and the interrupt is triggered, then RTS is automatically de-asserted (high), indicating there is no more room for data.
After data has been read out of the RX FIFO so it is filled less than the threshold, then the RTS is re-asserted (low).
After a specified number of bytes have been read, the application will pause transmission by not reading the RX FIFO and emulating a delay.
This will keep RTS de-asserted during the delay, and pausing transmission. After the delay, the RX FIFO is read, which will re-assert RTS and signal to continue transmission.
The RX Timeout interrupt is triggered after a set timeout. This will handle the scenario where the TX application has sent some final bytes, but it is not enough to pass the RX FIFO threshold, so the RX Timeout interrupt will trigger which will allow the application to read the final bytes.

After all the expected data in the first packet has been received, then the application disables automatic HW flow control. This allows the user to manually assert or de-assert the RTS signal.
The application manually asserts the RTS signal, and after receiving some bytes, de-asserts RTS and emulates a delay to pause transmission, and then re-asserts RTS to signal to continue tranmission.

After all data in the second packet has been received, a SW breakpoint is set to allow the user to inspect the data in the packets.

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
| PA9 | UART1 | RX | J1_3 | <ul><li>PA9 can be connected to J1_3<br><ul><li>`J1 2:3` Connect pin to J1_3<br><li>`J1 1:2/OFF` Disconnect pin from J1_3</ul> |
| PA8 | UART1 | TX | J1_4 | N/A |
| PB9 | UART1 | RTS | J1_7 | N/A |
| PB8 | UART1 | CTS | J2_15 | N/A |
| PA20 | DEBUGSS | SWCLK | N/A | <ul><li>PA20 is used by SWD during debugging<br><ul><li>`J101 15:16 ON` Connect to XDS-110 SWCLK while debugging<br><li>`J101 15:16 OFF` Disconnect from XDS-110 SWCLK if using pin in application</ul></ul> |
| PA19 | DEBUGSS | SWDIO | N/A | <ul><li>PA19 is used by SWD during debugging<br><ul><li>`J101 13:14 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J101 13:14 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0G3507 User's Guide](https://www.ti.com/lit/slau873).

## Example Usage
Connect this receiver device to a transmitter as follows:
- UART Receiver RX <- UART Transmitter TX
- UART Receiver RTS -> UART Transmitter CTS

Compile, load and run the example.
Optionally, connect a logic analyzer or oscilloscope to observe hardware flow control.
Ensure that the UART Receiver example is running before starting the UART Transmitter example.
Once the example is started, the UART Receiver will wait to receive data.
A breakpoint will be asserted after receiving the data.
