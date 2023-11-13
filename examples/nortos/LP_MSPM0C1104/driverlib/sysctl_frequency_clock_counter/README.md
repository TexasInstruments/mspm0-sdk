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

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA4 | Standard Output |
| SYSCTL | PA18 | CLK OUT |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0C1104](https://www.ti.com/tool/LP-MSPM0C1104) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA4 | GPIOA | PA4 | J2_14 | N/A |
| PA18 | SYSCTL | CLK_OUT | J2_15 | N/A |
| PA20 | DEBUGSS | SWCLK | J2_11 | <ul><li>PA20 is used by SWD during debugging<br><ul><li>`J101 13:14 ON` Connect to XDS-110 SWCLK while debugging<br><li>`J101 13:14 OFF` Disconnect from XDS-110 SWCLK if using pin in application</ul></ul> |
| PA19 | DEBUGSS | SWDIO | J2_17 | <ul><li>PA19 is used by SWD during debugging<br><ul><li>`J101 11:12 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J101 11:12 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0C1104 web page](https://www.ti.com/tool/LP-MSPM0C1104).

## Example Usage
Compile, load and run the example.
The red LED should turn on after executing if the FCC counter is within the
expected range, otherwise the LED will remain off if the test fails.
Confirm FCC mesurement by measuring SYSOSC frequency via CLK_OUT.
