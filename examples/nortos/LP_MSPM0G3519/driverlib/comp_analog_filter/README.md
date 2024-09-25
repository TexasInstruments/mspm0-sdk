## Example Summary

COMP is configured with external inputs and outputs enabling the analog output
filter and hysteresis.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | Pin | Launchpad Pin |
| --- | --- | --- | --- |
| VREF | VREF Positive (VREF+) | PA23 | J6_34 |
| COMP0 | COMP Output Pin | PB10 | J3_29 |
| COMP0 | COMP Positive Channel 0 Pin | PA26 | J3_24 |
| COMP0 | COMP Negative Channel 0 Pin | PA27 | J3_23 |
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
Connect COMP_PIN+ and COMP_IN- pins to external circuitry.
COMP_OUT will go HIGH if COMP_IN+ > COMP_IN-. Otherwise COMP_OUT is low.
Verify correct state is seen on COMP_OUT in.
