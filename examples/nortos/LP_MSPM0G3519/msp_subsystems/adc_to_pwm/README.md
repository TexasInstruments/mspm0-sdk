## Example Summary

This example demonstrates how to convert an analog signal to a 4kHz PWM output. The analog input signal is sampled using the MSPM0 integrated ADC. The duty cycle of the PWM output gets updated based on the ADC reading. Two timers are used in this example. One to trigger the ADC reading and another to generate the PWM output.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| SYSCTL |  |  |  |  |
| TIMG7 | Capture/Compare Pin 0 | PA17 | J7_25 |  |
| TIMG0 |  |  |  |  |
| ADC0 | ADC12 Channel 0 Pin | PA27 | J3_23 |  |
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

Compile, load and run the example. Apply a voltage from 0-3.3V to PA27, and monitor the PWM Duty cycle on PA17 as you change this voltage level.