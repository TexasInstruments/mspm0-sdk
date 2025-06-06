## Example Summary

This example demonstrates using the two switches on the MSPM0G3519 Launchpad to change the period and duty cycle on TIMA1 PWM output channels CC0 and CC1. Pressing switch S1 changes the PWM period of the timer. Pressing S2 changes the duty cycle on CC1 output. This example also demonstrates the use of TIMA0 configured as a one-shot timer and interrupts for debouncing the switches.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOA | Standard with Wake Input | PA18 | S1 | Populate Jumper(s): J1[1:2] |
| GPIOB | Standard Output | PB22 | LED2 Blue | Populate Jumper(s): J7[1:2] |
| GPIOB | Standard Output | PB27 | LED2 Green | Populate Jumper(s): J9[1:2] |
| GPIOB | Standard Output | PB26 | LED2 Red | Populate Jumper(s): J8[1:2] |
| GPIOB | Standard Input with internal pull-up | PB21 | J7_26 |  |
| SYSCTL |  |  |  |  |
| TIMA1 | Capture/Compare Pin 0 | PA10 | J4_34 | Populate Jumper(s): J14[2:1] |
| TIMA0 |  |  |  |  |
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
Compile, load, and run the example. At startup, TIMA1 is loaded with a PWM period of 1ms (1000Hz) and 75% duty cycle on CC0 and 25% duty cycle on CC1.  Pressing switch S1 will cycle through the following PWM periods: 1ms, 2ms, 4ms, and 0.5ms.    These changes are reflected on both CC0 and CC1 outputs. Pressing switch S2 will cycle through the following PWM duty cycles: 75%, 90%, 25%, and 50%

