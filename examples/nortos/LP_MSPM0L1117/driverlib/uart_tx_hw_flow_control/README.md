## Example Summary

This example can be used with the uart_rx_hw_flow_control example running on another device.
This example sets up the UART with hardware flow control.

The application will begin by sending the first packet of data.
Since hardware flow control is enabled, the transmitter will automatically check that the CTS signal isasserted (low) before transmitting the next byte.
If CTS is de-asserted (high), the byte will not be transmitted out of the TX FIFO. Therefore we block if the TX FIFO is full before attempting to fill it.

After the first packet has been transmitted, the application will delay for some time to allow the UART Receiver application to re-configure its flow control settings.
After the delay, the application will begin to send the second packet of data.

After all data has been transmitted, the application will go to sleep.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOA | Open-Drain Output | PA0 | Red LED1 | Populate Jumper(s): J1[1:2] |
| GPIOA | High-Drive Output | PA15 | J3_30 |  |
| SYSCTL |  |  |  |  |
| UART0 | RX Pin | PA11 | J4_33 | Populate Jumper(s): J12[2:3] |
| UART0 | RX Pin | PA11 | Back-Channel UART RXD | Populate Jumper(s): J12[2:1] |
| UART0 | TX Pin | PA10 | J4_34 | Populate Jumper(s): J10[2:3] |
| UART0 | TX Pin | PA10 | Back-Channel UART TXD | Populate Jumper(s): J10[2:1] |
| UART0 | RTS Pin | PA8 | J1_4 |  |
| UART0 | CTS Pin | PA9 | J1_3 |  |
| EVENT |  |  |  |  |
| BOARD | Debug Clock | PA20 | J22_16 |  |
| BOARD | Debug Data In Out | PA19 | J22_14 |  |


### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0L2228 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0L2228 web page](https://www.ti.com/tool/LP-MSPM0L2228).

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