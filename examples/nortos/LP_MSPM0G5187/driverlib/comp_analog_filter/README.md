## Example Summary

COMP is configured with external inputs and outputs enabling the analog output
filter and hysteresis.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| SYSCTL |  |  |  |  |
| VREF |  |  |  |  |
| COMP0 | COMP Output Pin | PB25 | J1_8 |  |
| COMP0 | COMP Positive Channel 0 Pin | PB21 | J1_5 |  |
| COMP0 | COMP Negative Channel 0 Pin | PB22 | J1_6 |  |
| EVENT |  |  |  |  |
| BOARD | Debug Clock | PA20 | J101_16 |  |
| BOARD | Debug Data In Out | PA19 | J101_14 |  |

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
MSPM0 LaunchPad, please visit the [LP-MSPM0G5187 User's Guide](https://www.ti.com/lit/slau967).

## Example Usage
Connect COMP_PIN+ and COMP_IN- pins to external circuitry.
COMP_OUT will go HIGH if COMP_IN+ > COMP_IN-. Otherwise COMP_OUT is low.
Verify correct state is seen on COMP_OUT in.
