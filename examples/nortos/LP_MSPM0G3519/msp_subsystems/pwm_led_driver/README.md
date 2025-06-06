## Example Summary

The following example configures TimerA2 to output an edge aligned PWM
signal that increases and decreases in duty cycle in a heartbeat manner to drive an LED.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| SYSCTL |  |  |  |  |
| TIMA0 | Capture/Compare Pin 0 | PA0 | J5_10 |  |
| EVENT |  |  |  |  |
| BOARD | Debug Clock | PA20 | J19_16 |  |
| BOARD | Debug Data In Out | PA19 | J19_14 |  |

Visit [LP_MSPM0G3519](https://www.ti.com/tool/LP-MSPM0G3519) for LaunchPad information, including user guide and hardware files.

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0G3519 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0G3519 User's Guide](https://www.ti.com/lit/slau947).

## Example Usage
Compile, load, and run the example.
TimerA0 will output a 62.5Hz edge aligned PWM signals on PA0.
Duty cycle of the PWM output will range from 10% to 90% in 1% increments.
Monitor the PWM signals on PA0.
