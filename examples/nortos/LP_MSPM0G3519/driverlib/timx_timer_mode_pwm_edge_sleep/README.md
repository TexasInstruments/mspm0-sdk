## Example Summary

The following example configures TimerG0 to output a pair of edge aligned PWM
signals. The device is configured to wait in SLEEP mode.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | Pin | Launchpad Pin |
| --- | --- | --- | --- |
| TIMG0 | Capture/Compare Pin 0 | PA12 | J4_32 |
| TIMG0 | Capture/Compare Pin 1 | PA13 | J4_31 |
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
TimerG0 will output a pair of 62.5Hz edge aligned PWM signals on PA12 and PA13.
PA12 is configured at a 75% duty cycle.
PA13 is configured at a 50% duty cycle.
Monitor the PWM signals on PA12 and PA13.
