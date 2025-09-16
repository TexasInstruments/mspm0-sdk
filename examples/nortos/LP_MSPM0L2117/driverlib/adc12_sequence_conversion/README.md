## Example Summary

The following example configures ADC0 to trigger sequence of conversions on 4 different channels and stores the results in 4 different buffer arrays.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| ADC0 | ADC12 Channel 0 Pin | PA27 | LCD-P4 |  |
| ADC0 | ADC12 Channel 7 Pin | PA22 | J1_6 |  |
| ADC0 | ADC12 Channel 2 Pin | PA25 | LCD-P2 |  |
| ADC0 | ADC12 Channel 3 Pin | PA24 | LCD-P1 |
| SYSCTL |  |  |  |  |
| EVENT |  |  |  |  |
| BOARD | Debug Clock | PA20 | J14_15 |  |
| BOARD | Debug Data In Out | PA19 | J14_13 |  |

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
MSPM0 LaunchPad, please visit the LP-MSPM02117 User's Guide.

## Example Usage

Compile, load and run the example. Connect vaild inputs ranging from 0-VCC to the ADC channels, floating pins are not guaranteed to read as 0.
