## Example Summary

The following example configures TimerG0 to output a pair of edge aligned PWM
signals. The device is configured to wait in SLEEP mode.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| SYSCTL |  |  |  |  |
| TIMG14 | Capture/Compare Pin 0 | PA0 | J4_34 |  |
| TIMG14 | Capture/Compare Pin 1 | PA11 | J14_7 |  |
| EVENT |  |  |  |  |
| BOARD | Debug Clock | PA20 | J14_4 |  |
| BOARD | Debug Data In Out | PA19 | J14_2 |  |

### Device Migration Recommendations
This project was developed for a superset device included in the LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the LP-MSPM0C1106 User's Guide.

## Example Usage
Compile, load and run the example.
TimerG14 will output a pair of 62.5Hz edge aligned PWM signals on PA0 and PA6.
PA0 is configured at a 75% duty cycle.
PA6 is configured at a 50% duty cycle.
Monitor the PWM signals on PA0 and PA6.
