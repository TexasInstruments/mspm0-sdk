## Example Summary

C++ example toggling four GPIO pins.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOA | Standard with Wake Output | PA17 | LED2 Red | Populate Jumper(s): J12[2:1] |
| GPIOA | Standard Output | PA24 | LED2 Green | Populate Jumper(s): J13[2:1] |
| GPIOA | Standard Output | PA25 | J1_2 |  |
| GPIOB | Standard Output | PB13 | LED2 Blue | Populate Jumper(s): J11[2:1] |
| SYSCTL |  |  |  |  |
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
MSPM0 LaunchPad, please visit the [LP-MSPM0G5187 User's Guide](https://www.ti.com/lit/slau967).

## Example Usage
Compile, load and run the example.
RGB LEDs will toggle with red being opposite of blue and green. USER_TEST_PIN
GPIO will mimic the behavior of the LED1 and LED3 pins on the BoosterPack
header and can be used to verify the LED behavior.
