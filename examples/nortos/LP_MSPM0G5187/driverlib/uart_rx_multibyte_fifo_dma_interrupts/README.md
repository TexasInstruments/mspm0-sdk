## Example Summary

This example sets up the UART to wait to receive data from another UART device using DMA.
The example can be used with the uart_tx_multibyte_fifo_dma_interrupts example running on another device.
The DMA in configured in single transfer mode, using the static UART RX DMA event as the DMA trigger.
When data is received in the UART RXFIFO, this triggers the DMA to transfer the data from the UART RXFIFO to a buffer.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOA | Standard with Wake Output | PA17 | LED2 Red | Populate Jumper(s): J12[2:1] |
| SYSCTL |  |  |  |  |
| UC0 | RX Pin | PA11 | Back-Channel UART RXD |  |
| UC0 | RX Pin | PA11 | J4_33 |  |
| UC0 | TX Pin | PA10 | Back-Channel UART TXD |  |
| UC0 | TX Pin | PA10 | J4_34 |  |
| EVENT |  |  |  |  |
| DMA |  |  |  |  |
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
Compile, load and run the example.
Connect the RX and TX between two UART devices or to the Launchpad's back-channel UART.
The example will wait to receive 4 bytes of data from the transmitting device.
The LED will turn ON to indicate that the example has completed without errors.
The USER_TEST pin will match the LED pin state.
The received data can be verified in the reception buffer, gRxPacket.
