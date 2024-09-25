## Example Summary

The following example configures ADC0 and ADC1 to be simultaneously triggered
by a TimerG instance 1s after the timer has been started. Each instance is
monitoring and independent event subscriber channel.

This example also showcases how to enable HW averaging. ADC0 instance is
configured to accumulate 128 sample and then left shift the accumulated result
by 128 (Right shift results by 7). ADC1 instance is configured to accumulate 32
sample and then left shift the accumulated result by 32 (Right shift results by
5).

Once both conversions are done both results are compared and LED1 is set if
(adc0Res > adc1Res).

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | Pin | Launchpad Pin |
| --- | --- | --- | --- |
| GPIOA | Open-Drain Output | PA0 | Red LED1 |
| ADC0 | ADC12 Channel 12 Pin | PA14 | J1_2 |
| ADC1 | ADC12 Channel 14 Pin | PB27 | J3_28 |
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
Compile, load and run the example. Provide a valid voltage to both ADC
pins and monitor LED1 (Voltage source for ADC instances is excepted to be between
0v and MCU supply voltage).
