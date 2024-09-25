## Example Summary

This example demonstrates how to simultaneously service three GPIO interrupts.
The corresponding LED will be toggled for each GPIO interrupt.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | Pin | Launchpad Pin |
| --- | --- | --- | --- |
| GPIOB | Standard Output | PB22 | LED2 Blue |
| GPIOB | Standard Output | PB26 | LED2 Red |
| GPIOB | Standard Output | PB27 | LED2 Green |
| GPIOB | Standard Input with internal pull-down | PB12 | J2_13 |
| GPIOB | Standard Input with internal pull-down | PB11 | J2_12 |
| GPIOB | Standard Input with internal pull-down | PB14 | J2_11 |
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
Compile, load and run the example.
The LEDs will toggle based on the serviced GPIO.

For demo purposes, to perform simultaneous interrupts connect the input pins to the same input signal.
