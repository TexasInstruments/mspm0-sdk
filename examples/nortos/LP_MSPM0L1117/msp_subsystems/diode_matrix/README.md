## Example Summary

Uses 6 GPIO pins to control a 3 by 3 common row cathode diode matrix. See block diagram for setup and signal connections.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOA | Standard Output | PA5 | S2 | Populate Jumper(s): J6[1:2] |
| GPIOA | Standard Output | PA4 | J21_5 | Populate Component(s): R12,J21 |
| GPIOA | Standard Output | PA3 | J21_7 | Populate Component(s): R11,J21 |
| GPIOA | Standard Output | PA12 | J3_25 |  |
| GPIOA | Standard Output | PA13 | J2_14 |  |
| GPIOA | Standard Output | PA27 | J1_8 |  |
| SYSCTL |  |  |  |  |
| TIMG0 |  |  |  |  |
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
Connect column and row pins to diode matrix. Compile, load and run the example.
"T" and "I" will flash on repeat.
