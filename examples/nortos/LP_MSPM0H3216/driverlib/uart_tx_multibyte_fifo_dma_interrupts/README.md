## Example Summary

This example sets up the UART to trigger the DMA to do a transmit data transfer.
The example can be used with the uart_rx_multibyte_fifo_dma_interrupts example running on another device.
The DMA in configured in single transfer mode, using the static UART TX DMA event as the DMA trigger.
When the UART TXINT is set, this triggers the DMA to transfer the data from a buffer to the UART TXFIFO.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOA | Standard Output | PA0 | Red LED1 | Populate Jumper(s): J4[1:2] |
| GPIOA | Standard Output | PA15 | J4_38 |  |
| SYSCTL |  |  |  |  |
| UART0 | RX Pin | PB7 | J1_3 |  |
| UART0 | TX Pin | PB6 | J1_4 |  |
| EVENT |  |  |  |  |
| DMA |  |  |  |  |
| BOARD | Debug Clock | PA20 | J13_4 | Populate Component(s): J13 |
| BOARD | Debug Data In Out | PA19 | J13_2 | Populate Component(s): J13 |

### Device Migration Recommendations
This project was developed for a superset device included in the MSPM0 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the LP-MSPM0H3216 User's Guide.

## Example Usage
Compile, load and run the example.
Connect the RX and TX between two UART devices or to the Launchpad's back-channel UART.
The example will transmit data.
Check the received data in a terminal or the receiver device.

The LED will turn on to indicate that the example has completed without any errors.
USER_TEST_PIN GPIO will mimic the behavior of the LED pin on the BoosterPack
header and can be used to verify the LED behavior.
