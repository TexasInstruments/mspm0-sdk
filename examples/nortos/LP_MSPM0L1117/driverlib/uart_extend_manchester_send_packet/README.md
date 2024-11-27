## Example Summary

This example demonstrates UART TX and RX with Manchester encoding. Manchester encoding is a feature only usable with UART Extend.

This example can be used with the uart_extend_manchester_send_packet example running on another device.

This example enables Manchester encoding and decoding, then blocks on waiting to receive a packet of data. When the expected number of bytes has been received, it echos back the received data. The LED then toggles to indicate success.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOA | Open-Drain Output | PA0 | Red LED1 | Populate Jumper(s): J1[1:2] |
| SYSCTL |  |  |  |  |
| UART0 | RX Pin | PA11 | J4_33 | Populate Jumper(s): J12[2:3] |
| UART0 | RX Pin | PA11 | Back-Channel UART RXD | Populate Jumper(s): J12[2:1] |
| UART0 | TX Pin | PA10 | J4_34 | Populate Jumper(s): J10[2:3] |
| UART0 | TX Pin | PA10 | Back-Channel UART TXD | Populate Jumper(s): J10[2:1] |
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
Connect the RX and TX between two UART devices.
Ensure that this example is running first, before the uart_extend_manchester_send_packet is started.
Compile, load and run the example.
This example will wait to receive UART data from another device, then toggle the LED upon reception.
