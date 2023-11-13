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
| PB26 | GPIOB | PB26 | J27_8 | <ul><li>PB26 can be connected to LED2 Red<br><ul><li>`J6 ON` Connect to LED2 Red<br><li>`J6 OFF` Disconnect from LED2 Red</ul></ul> |
| PB27 | GPIOB | PB27 | J27_10 | <ul><li>PB27 can be connected to LED2 Green<br><ul><li>`J7 ON` Connect to LED2 Green<br><li>`J7 OFF` Disconnect from LED2 Green</ul></ul> |
| PA14 | SYSCTL | CLK_OUT | J28_7 | N/A |
| PA3 | SYSCTL | LFXIN | J28_2 | <ul><li>PA3 is used for LFXT functionality by default in Launchpad<br><ul><li>`Y1` crystal is soldered and connected by default</ul><br><li>PA3 can be connected to J28_2<br><ul><li>`R10` is not soldered soldered by default<br><li>Solder `R10` to connect pin to J28_2</ul> |
| PA4 | SYSCTL | LFXOUT | J28_3 | <ul><li>PA4 is used for LFXT functionality by default in Launchpad<br><ul><li>`Y1` crystal is soldered and connected by default</ul><br><li>PA4 can be connected to J28_3<br><ul><li>`R11` is not soldered soldered by default<br><li>Solder `R11` to connect pin to J28_3</ul> |
| PA20 | DEBUGSS | SWCLK | N/A | <ul><li>PA20 is used by SWD during debugging<br><ul><li>`J101 15:16 ON` Connect to XDS-110 SWCLK while debugging<br><li>`J101 15:16 OFF` Disconnect from XDS-110 SWCLK if using pin in application</ul></ul> |
| PA19 | DEBUGSS | SWDIO | N/A | <ul><li>PA19 is used by SWD during debugging<br><ul><li>`J101 13:14 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J101 13:14 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0G3507 User's Guide](https://www.ti.com/lit/slau873).

## Example Usage
Compile, load and run the example.
The green LED should turn on after executing if the FCC counter is within the
expected range.
Confirm FCC mesurement by measuring SYSOSC frequency via CLK_OUT.
