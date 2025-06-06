## Example Summary

This example shows how to use MSPM0 to drive TPS92543 to control multiple LEDs. The host computer sends different instructions to MSPM0 through CANFD, and then MSPM0 controls TPS92543 through the UART/CAN interface to achieve different patterns.

The example uses a TPS92543 which is a 6-channel, 65V high-side LED driver that controls the 8-bit output current and 10-bit PWM duty cycles.

## Peripherals & Pin Assignments

| Peripheral | Pin  | Function                                       |
| ---------- | ---- | ---------------------------------------------- |
| GPIOB      | PB0  | Standard Output |
| GPIOB      | PB10 | Standard Output |
| SYSCTL     | PA5 |HFXIN                 |
| SYSCTL     | PA6 |HFXOUT                |
| TIMA0      | N/A  | Timeout Timer                          |
| UART0      | PA11 | RX Pin                                         |
| UART0      | PA10 | TX Pin                                         |
| CANFD      | PA13  | RX Pin                                         |
| CANFD      | PA12  | TX Pin                                         |
| DMA        |   N/A   | UART0 send and receive DMA with TPS92543   |
| DEBUGSS    | PA20 | Debug Clock                                    |
| DEBUGSS    | PA19 | Debug Data In Out                              |


### Device Migration Recommendations

This project was developed for a superset device included in the LP_MSPM0G3519 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations

TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0G3519 User's Guide](https://www.ti.com/lit/pdf/slau947).

## Example Usage
Connect the CAN_RX and CAN_TX to TCAN1046EVM pins as described in [TCAN Evaluation Module User's Guide](https://www.ti.com/lit/pdf/sllu296). Connect the PB0, PB10, UART_RX and UART_TX to TPS92543EVM pins
Compile, load and run the example.

CANFD:


The example has the following Nominal and Data Rate frequencies configured:

Nominal Bit Rate (250 KBits/s)
Data Bit Rate (2 MBits/s)

To enable LED, the MSPM0 must receive the following package:
- ID : 0x0C
- DATA[0] = 0x01

To change LED patterns, the MSPM0 must receive the following package:
- ID : 0x0C
- DATA[0] = 0x02 or 0x03

To disable LED, the MSPM0 must receive the following package:
- ID : 0x0C
- DATA[0] = 0x04
