## Example Summary

Write to LCD memory register to display characters on onboard LCD panel.

The LCD panel will display characters sequentially in a loop. The displayed characters will first loop through digits from 0-9 and then display letters from A-Z. Characters will cycle through position 1-6 on the LCD panel.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| SYSCTL |  |  |  |  |
| LCD |  |  |  |  |
| BOARD | Debug Clock | PA20 | J14_15 |  |
| BOARD | Debug Data In Out | PA19 | J14_13 |  |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0L2117 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the LP-MSPM0L2117 User's Guide.

## Example Usage

Compile, load and run the example.

The LCD panel will display characters sequentially in a loop. The displayed characters will first loop through digits from 0-9 and then display letters from A-Z. Characters will cycle through position 1-6 on the LCD panel.

The example uses LCD mode 3 to configure the LCD.
