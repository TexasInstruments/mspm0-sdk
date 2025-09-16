## Example Summary
This example demonstrate how to send a CAN-FD message via GPIO input interrupt.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | Pin | Launchpad Pin |
| --- | --- | --- | --- |
| GPIOB | Standard Input with internal pull-up | PB21 | J7_26 |
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
Evaluation Module User's Guide and monitor and receive CAN-FD packages using
mcan_single_message_rx example project or using a CAN Bus Analyzer tool of your
choice.

Compile, load and run the example.

The example has the following Nominal and Data Rate frequencies configured:
- Nominal Bit Rate (250 kbps)
- Data Bit Rate    (2 Mbps)

When S2 button is pressed (or the GPIO detects a falling input edge), the
following CAN-FD packages will be transmitted (Message 1 and 2 are toggled
every time S2 button is pressed).

Message 1:
- ID : 0x04
- BRS: 0x01
- DLC: 0x01
- FDF: 0x01
- DATA[0] = 0x01

Message 2:
- ID : 0x04
- BRS: 0x01
- DLC: 0x01
- FDF: 0x01
- DATA[0] = 0x00
