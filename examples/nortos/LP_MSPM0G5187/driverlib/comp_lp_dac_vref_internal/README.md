## Example Summary

COMP is configured in low-power mode using the 8-bit DAC with internal reference
as positive input, and the external COMP as negative input.
An interrupt is triggered when the external input crosses the DAC treshold and
a LED is toggled accordingly.
The device stays in STOP mode, except when an interrupt is fired.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOA | Open-Drain Output | PA0 | Red LED1 | Populate Jumper(s): J9[2:1] |
| SYSCTL |  |  |  |  |
| VREF |  |  |  |  |
| COMP0 | COMP Positive Channel 0 Pin | PB21 | J1_5 |  |
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

Connect COMP_IN+ to an external signal. COMP_IN- is internally connected to
internal VREF+ (2.5V).
Compile, load and run the example.
LED is turned on when COMP_IN+ falls below COMP_IN-.
LED is turned off when COMP_IN+ raises above COMP_IN+.
