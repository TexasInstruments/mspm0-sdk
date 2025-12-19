## Example Summary

COMP is configured to trigger a PWM fault when positive input exceeds AVCC/2.
The 8-bit DAC is used to set the negative threshold of the comparator while the
positive input is connected to an external pin.
A PWM will be driven when the comparator input is below AVCC/2 and then stop
when the input goes above this threshold.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| SYSCTL |  |  |  |  |
| TIMA0 | Capture/Compare Pin 0 | PB3 | J1_10 |  |
| COMP0 | COMP Positive Channel 0 Pin | PA26 | J1_5 |  |
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
MSPM0 LaunchPad, please visit the LP-MSPM0G3218 User's Guide.

## Example Usage
Connect COMP0_IN0+ to an external signal.
Compile, load and run the example.
Check PWM signals using an oscilloscope.
PWM will stop when the comparator input exceeds AVCC/2.

On powerup, the COMP pin(s) used in this example are by default set to
the correct analog mode. Therefore, calls to
DL_GPIO_initPeripheralAnalogFunction for pinmuxing those pins are not needed.
