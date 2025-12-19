## Example Summary

This example demonstrates UART TX and RX with Manchester encoding. Manchester encoding is a feature only usable with UART Extend.

This example can be used with the uart_extend_manchester_echo example running on another device.

This example enables Manchester encoding and decoding, then transmits a packet of data. The device then blocks on waiting for the packet data to be echoed back.
When the expected number of bytes has been received, it checks to see that the transmitted data matches the received data. If the data does not match, a software breakpoint is set. Otherwise, an LED is toggled to indicate success.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOA | Open-Drain Output | PA0 | Red LED1 | Populate Jumper(s): J4[1:2] |
| UC0 | RX Pin | PA11 | Back-Channel UART RXD | Populate Jumper(s): J22[2:1] |
| UC0 | RX Pin | PA11 | J2_33 | Populate Jumper(s): J22[2:3] |
| UC0 | TX Pin | PA10 | Back-Channel UART TXD | Populate Jumper(s): J21[2:1] |
| UC0 | TX Pin | PA10 | J2_34 | Populate Jumper(s): J21[2:3] |
| EVENT |  |  |  |  |
| BOARD | Debug Clock | PA20 | J101_16 |  |
| BOARD | Debug Data In Out | PA19 | J101_14 |  |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0G3218 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0G3218 User's Guide](https://www.ti.com/lit/slau873).

## Example Usage
Connect the RX and TX between two UART devices.
Ensure that the uart_extend_manchester_echo is running first on another device.
Compile, load and run the example.
The LED should toggle after every packet received correctly after an echo from the other device.
