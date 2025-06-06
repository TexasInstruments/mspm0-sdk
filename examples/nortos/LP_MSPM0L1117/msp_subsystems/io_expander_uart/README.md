## Example Summary

Capture several GPIOs' status, update and send the status to host through UART.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOA | Standard Input with internal pull-up | PA14 | J1_6 |  |
| GPIOA | Standard Input with internal pull-down | PA13 | J2_14 |  |
| GPIOA | High-Drive Input with internal pull-down | PA15 | J3_30 |  |
| GPIOA | High-Drive Input with internal pull-down | PA16 | J3_29 | Populate Jumper(s): J15[1:2] |
| GPIOA | High-Drive Input with internal pull-down | PA16 | S1 | Populate Jumper(s): J15[1:2],J15[2:3],J2[1:2] |
| GPIOA | High-Drive Input with internal pull-down | PA17 | J3_28 |  |
| GPIOA | High-Drive Input with internal pull-down | PA18 | J3_26 |  |
| GPIOA | Standard Input with internal pull-down | PA12 | J3_25 |  |
| GPIOA | High-Drive Input with internal pull-down | PA11 | J4_33 | Populate Jumper(s): J12[2:3] |
| GPIOA | High-Drive Input with internal pull-down | PA10 | J4_34 | Populate Jumper(s): J10[2:3] |
| SYSCTL |  |  |  |  |
| TIMG0 |  |  |  |  |
| UART0 | RX Pin | PA1 | J1_9 |  |
| UART0 | TX Pin | PA0 | J1_10 |  |
| EVENT |  |  |  |  |
| BOARD | Debug Clock | PA20 | J22_16 |  |
| BOARD | Debug Data In Out | PA19 | J22_14 |  |

Visit [LP_MSPM0L1117](https://www.ti.com/tool/LP-MSPM0L1117) for LaunchPad information, including user guide and hardware files.

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0L1117 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0L1117 User's Guide](https://www.ti.com/lit/slau953).

## Example Usage
Compile, load and run the example.
Capture several GPIOs' status, update and send the status to host through UART.
