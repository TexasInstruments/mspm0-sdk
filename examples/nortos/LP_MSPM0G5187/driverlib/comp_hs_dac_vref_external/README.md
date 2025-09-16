## Example Summary

COMP is configured in high-speed mode using the 8-bit DAC with external
reference (VREF+) as the positive input, and the external pin (COMP_IN-)
as negative input. An interrupt is triggered when the external input crosses
the DAC treshold and a LED is toggled accordingly.
The COMP hysteresis and filter are used to prevent multiple interrupt firing
immediately.
The device stays in SLEEP mode, except when an interrupt is fired.

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

0 Ohmn resistors must be soldered connect VREF pins to boosterpack.
See device schematic for more details.

Connect COMP_IN- to an external signal.
Connect 3V external reference to VREF+.

Compile, load and run the example.
Check LED toggle when signal connected to COMP_IN- rises above or below 1V.
