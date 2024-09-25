## Example Summary

This example demonstrates UART TX and RX with IrDA encoding. IrDA encoding is a feature only usable with UART Extend.

This example can be used with the uart_extend_irda_receive_packet example running on another device.

This example enables IrDA encoding and decoding, then transmits a packet of data. The device then waits at a breakpoint if the transmission is successful.
Connect the TX line to the RX line of a device running the uart_extend_irda_receive_packet and see if the other device receives the correct message.

**NOTE:** An IrDA transceiver can also be used to further ensure that the IrDA encoding and decoding is functional.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | Pin | Launchpad Pin |
| --- | --- | --- | --- |
| GPIOA | Open-Drain Output | PA0 | Red LED1 |
| UART0 | RX Pin | PA11 | J19_8 (Default) or J4_33 |
| UART0 | TX Pin | PA10 | J19_10 (Default) or J4_34 |
| BOARD | Debug Clock | PA20 | J19_16 |
| BOARD | Debug Data In Out | PA19 | J19_14 |

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
MSPM0 LaunchPad, please visit the LP-MSPM0G3519 User's Guide.

## Example Usage
Connect the RX and TX between two UART devices.
Ensure that the uart_extend_irda_receive_packet is running first on another device.
Compile, load and run the example.
The LED should toggle after packet is received.
