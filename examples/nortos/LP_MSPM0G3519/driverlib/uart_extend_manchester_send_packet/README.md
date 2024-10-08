## Example Summary

This example demonstrates UART TX and RX with Manchester encoding. Manchester encoding is a feature only usable with UART Extend.

This example can be used with the uart_extend_manchester_echo example running on another device.

This example enables Manchester encoding and decoding, then transmits a packet of data. The device then blocks on waiting for the packet data to be echoed back.
When the expected number of bytes has been received, it checks to see that the transmitted data matches the received data. If the data does not match, a software breakpoint is set. Otherwise, an LED is toggled to indicate success.

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
Ensure that the uart_extend_manchester_echo is running first on another device.
Compile, load and run the example.
The LED should toggle after every packet received correctly after an echo from the other device.
