## Example Summary

Demonstrates Frequency Clock Counter (FCC). The FCC is configured to count the
number of SYSOSC periods that have occurred between two LFCLK rising-edge to
rising-edge periods.

Once the FCC is done counting the period, the number of measured counts is
compared against pre-computed max and min expected values to determine if
SYSOSC is within the expected frequency range.

The red LED is turned on if the FCC counter is within the expected range or will
remain off if the test fails.

The example is also configured to output SYSOSC via CLK_OUT to allow monitoring
the clock externally.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | Pin | Launchpad Pin |
| --- | --- | --- | --- |
| GPIOA | Open-Drain Output | PA0 | Red LED1 |
| GPIOA | Standard Output | PA15 | J3_30 |
| SYSCTL | CLK OUT | PA14 | J1_2 |
| SYSCTL | LFXIN | PA3 | N/A |
| SYSCTL | LFXOUT | PA4 | N/A |
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
Compile, load and run the example.
LED1 should turn on after executing if the FCC counter is within the
expected range. USER_TEST_PIN GPIO will mimic the behavior of the LED pin on the
BoosterPack header and can be used to verify the LED behavior.
Confirm FCC measurement by measuring SYSOSC frequency via CLK_OUT.