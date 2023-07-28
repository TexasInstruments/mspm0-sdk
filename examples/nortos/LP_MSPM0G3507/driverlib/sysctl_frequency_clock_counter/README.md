## Example Summary

Demonstrates Frequency Clock Counter (FCC). The FCC is configured to count the
number of SYSOSC periods that have occurred between two LFXT rising-edge to
rising-edge periods.

Once the FCC is done counting the period, the number of measured counts is
compared against pre-computed max and min expected values to determine if
SYSOSC is within the expected frequency range.

The green LED is turned on if the FCC counter is within the expected range.
The red LED is turned on if the FCC counter is outside the expected range.

The example is also configured to output SYSOSC via CLK_OUT to allow monitoring
the clock externally.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOB | PB26 | Standard Output |
| GPIOB | PB27 | Standard Output |
| SYSCTL | PA14 | CLK OUT |
| SYSCTL | PA3 | LFXIN |
| SYSCTL | PA4 | LFXOUT |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PB26 | GPIOB | PB26 | J27_8 | J6 ON/OFF Connect/Disconnect LED2 Red |
| PB27 | GPIOB | PB27 | J27_10 | J7 ON/OFF Connect/Disconnect LED2 Green |
| PA14 | SYSCTL | CLK_OUT | J28_7 | N/A |
| PA3 | SYSCTL | LFXIN | J28_2 | Connected to LFXT Y1<br>Solder R10 to connect to J28_2 |
| PA4 | SYSCTL | LFXOUT | J28_3 | Connected to LFXT Y1<br>Solder R11 to connect to J28_3 |
| PA20 | DEBUGSS | SWCLK | N/A | J101 15:16 ON: Connect to XDS-110 SWCLK (debug) |
| PA19 | DEBUGSS | SWDIO | N/A | J101 13:14 ON: Connect to XDS-110 SWDIO (debug) |

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0G3507 User's Guide](https://www.ti.com/lit/slau846).

## Example Usage
Compile, load and run the example.
The green LED should turn on after executing if the FCC counter is within the
expected range.
Confirm FCC mesurement by measuring SYSOSC frequency via CLK_OUT.
