## Example Summary

The following example configures ADC0 to trigger single conversion by software
and turn on LED1 if ADC > 0.5*(external vref). Otherwise, LED1 remains off.

This examples configure ADC12 instance to use internal voltage reference.
## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOA | Open-Drain Output | PA0 | Red LED1 | Populate Jumper(s): J4[1:2] |
| SYSCTL |  |  |  |  |
| ADC0 | ADC12 Channel 2 Pin | PA25 | J1_7 |  |
| VREF | VREF Positive (VREF+) | PA23 | J1_5 |  |
| VREF | VREF Negative (VREF-) | PA21 | J14_6 |  |
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
Compile, load and run the example. Provide a valid voltage to ADC0_0 (Voltage
source for ADC is excepted to be between 0 - External Vref (external reference
shouldn't exceed VDD) and monitor LED1. LED1 will be turned on when ADC
conversion greater than (External Vref / 2).

On powerup, the ADC and VREF pin(s) used in this example are by default set to
the correct analog mode. Therefore, calls to
DL_GPIO_initPeripheralAnalogFunction for pinmuxing those pins are not needed.
