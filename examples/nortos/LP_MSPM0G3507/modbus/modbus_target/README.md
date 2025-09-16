## Example Summary

This example act as a Modbus Target and responds to the Function codes received over UART from controller.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | Pin | 
| --- | --- | --- |
| GPIOA | Open-Drain Output | PA0 |
| UART RX | UART recieve line | PA11 | 
| UART TX | UART transmit line | PA10 | 
| BOARD | Debug Clock | PA20 |
| BOARD | Debug Data In Out | PA19 |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0G3519 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#manual-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0G3507 web page](https://www.ti.com/tool/LP-MSPM0G3507).

## Example Usage

Connect the UART RX and UART TX pins to corresponding UART RX and UART TX pins of modbus controller. Compile, load and run the example. The target waits for the command and will respond to the commands accordingly. Check response using logic analyzer or set breakpoints to debug.
