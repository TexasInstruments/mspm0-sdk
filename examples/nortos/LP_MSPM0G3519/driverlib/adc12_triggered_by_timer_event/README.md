## Example Summary

The following example configures ADC0 to be triggered by a TimerG instance 1s
after the timer has been started. The TimerG event is published on channel 1.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | Pin | Launchpad Pin |
| --- | --- | --- | --- |
| ADC0 | ADC12 Channel 12 Pin | PA14 | J1_2 |
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
Provide a valid voltage to A0_2 (Voltage source for ADC is excepted to be
between 0V and MCU supply voltage). Compile, load and run the example. The
example will hit the breakpoint after the ADC conversion is done automatically.
Once it hits the break point gADCResult array can be inspected.

On powerup, the ADC pin(s) used in this example are by default set to
the correct analog mode. Therefore, calls to
DL_GPIO_initPeripheralAnalogFunction for pinmuxing those pins are not needed.
