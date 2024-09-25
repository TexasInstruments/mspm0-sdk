## Example Summary
This example demonstrates how to receive CAN Classic and CAN-FD
messages using standard IDs.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | Pin | Launchpad Pin |
| --- | --- | --- | --- |
| GPIOB | Standard Output | PB22 | LED2 Blue |
| GPIOB | Standard Output | PB26 | LED2 Red |
| GPIOB | Standard Output | PB13 | J8_19 |
| SYSCTL | HFXIN | PA5 | N/A |
| SYSCTL | HFXOUT | PA6 | N/A |
| CANFD0 | RX Pin | PA13 | J4_31 |
| CANFD0 | TX Pin | PA12 | J4_32 |
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
Connect the CAN_RX and CAN_TX to TCAN1042DEVM pins as described in TCAN
Evaluation Module User's Guide and monitor and transmit CAN-FD packages using
mcan_single_message_tx example project or using a CAN Bus Analyzer tool of your
choice.

The example has the following Nominal and Data Rate frequencies configured:
- Nominal Bit Rate (250 KBits/s)
- Data Bit Rate    (2 MBits/s)

Compile, load and run the example.

To turn on the LED_1, the example must receive the following package:
- ID : 0x03
- DATA[0] = 0x01

To turn off the LED_1, the example must receive the following package:
- ID : 0x03
- DATA[0] = 0x00

To turn on the LED_2, the example must receive the following package:
- ID : 0x04
- DATA[0] = 0x01

To turn off the LED_2, the example must receive the following package:
- ID : 0x04
- DATA[0] = 0x00

Note: There is a known limitation where the example might stop receiving
packages if the application halt execution after hitting a breakpoint. It is
highly recommended to use free run option while testing the example.
