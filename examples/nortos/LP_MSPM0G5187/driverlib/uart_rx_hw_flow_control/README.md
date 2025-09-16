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

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOA | Standard with Wake Output | PA17 | LED2 Red | Populate Jumper(s): J12[2:1] |
| SYSCTL |  |  |  |  |
| UC0 | RX Pin | PA11 | J4_33 |  |
| UC0 | TX Pin | PA10 | J4_34 |  |
| UC0 | RTS Pin | PA12 | J3_28 |  |
| UC0 | CTS Pin | PA14 | J3_30 |  |
| EVENT |  |  |  |  |
| BOARD | Debug Clock | PA20 | J101_16 |  |
| BOARD | Debug Data In Out | PA19 | J101_14 |  |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0G5187 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0G5187 User's Guide](https://www.ti.com/lit/slau873).

## Example Usage
Connect this receiver device to a transmitter as follows:
- UART Receiver RX <- UART Transmitter TX
- UART Receiver RTS -> UART Transmitter CTS

Compile, load and run the example.
Optionally, connect a logic analyzer or oscilloscope to observe hardware flow control.
Ensure that the UART Receiver example is running before starting the UART Transmitter example.
Once the example is started, the UART Receiver will wait to receive data.
A breakpoint will be asserted after receiving the data.
LED1 will turn on when the application completes. USER_TEST_PIN GPIO will mimic the
behavior of the LED pin on the BoosterPack header and can be used to verify the
LED behavior.
