## Example Summary

HFXT (40MHz in LaunchPad) is a source for HFCLK, which sources MCLK.
HFCLK/4 is output to CLK_OUT pin that can be measured.
HFXT -> HFCLK (4-48MHz) -> HSCLK -> MCLK -> /4 -> CLK_OUT

TimerG is configured in periodic mode and toggles an LED every 500 ms.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA0 | Open-Drain Output |
| SYSCTL | PA14 | CLK OUT |
| SYSCTL | PA5 | HFXIN |
| SYSCTL | PA6 | HFXOUT |
| TIMG0 |  |  |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA0 | GPIOA | PA0 | J27_9 | This pin is 5V tolerant open-drain and requires pull-up.<br>J4 ON/OFF: Connect/Disconnect LED1<br>J19 1:2 3.3V Pull-up<br>J19 2:3 5V Pull-up |
| PA14 | SYSCTL | CLK_OUT | J28_7 | N/A |
| PA5 | SYSCTL | HFXIN | J28_4 | Connected to HFXT  Y2<br>Solder R15 to connect to J28_4 |
| PA6 | SYSCTL | HFXOUT | J28_5 | Connected to HFXT  Y2<br>Solder R17 to connect to J28_5 |
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
Compile, load and run the example. LED toggles every 500 ms.
Measure the digital output of CLK_OUT.
