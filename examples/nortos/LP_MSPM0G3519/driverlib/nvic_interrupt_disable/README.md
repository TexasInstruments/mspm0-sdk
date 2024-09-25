## Example Summary

The following example configures configures TimerG in one shot mode, toggles an LED for 5 seconds, and then turns off the LED.
The peripheral interrupt is disabled and then the device is put to SLEEP.
This example demonstrates the best approach to disable interrupts.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | Pin | Launchpad Pin |
| --- | --- | --- | --- |
| GPIOA | Open-Drain Output | PA0 | Red LED1 |
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
LED0 will toggle on and off for 5 seconds.
After 5 seconds, the LED will turn off and the device will go to SLEEP.

## Application Design Details
When disabling interrupts, the best approach is to disable the peripheral
interrupts instead of just disabling the NVIC.
If just the NVIC is disabled and the peripheral interrupt is left enabled,
then the device will still wake up the power domain to check the interrupt status.
