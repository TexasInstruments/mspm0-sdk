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

Visit [LP_MSPM0L2228](https://www.ti.com/tool/LP-MSPM0L2228) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA0 | GPIOA | PA0 | J3_10 | <ul><li>PA0 is 5V tolerant open-drain so it requires pull-up<br><ul><li>`J12 1:2` Use 3.3V pull-up<br><li>`J12 2:3` Use 5V pull-up</ul><br><li>PA0 can be connected to LED3<br><ul><li>`J2 ON` Connect to LED3<br><li>`J2 OFF` Disconnect from LED3</ul></ul> |
| PA14 | SYSCTL | CLK_OUT | J3_26 | <ul><li>PA14 has a capacitor to be used for LCDCAP1 functionality by default in Launchpad<br><ul><li>Remove `C50` if needed to use pin for other functions</ul></ul> |
| PA5 | SYSCTL | HFXIN | J11_2 | <ul><li>PA5 is used for HFXT functionality by default in Launchpad<br><ul><li>`Y2` crystal is soldered and connected by default</ul><br><li>PA5 can be connected to J11_2<br><ul><li>`R45` and `J11` are not soldered soldered by default<br><li>Solder `R45` and `J11` to connect pin to J11_1</ul> |
| PA6 | SYSCTL | HFXOUT | J11_17 | <ul><li>PA6 is used for HFXT functionality by default in Launchpad<br><ul><li>`Y2` crystal is soldered and connected by default</ul><br><li>PA6 can be connected to J11_17<br><ul><li>`R46` and `J11` are not soldered soldered by default<br><li>Solder `R46` and `J11` to connect pin to J11_17</ul> |
| PA20 | DEBUGSS | SWCLK | N/A | <ul><li>PA20 is used by SWD during debugging<br><ul><li>`J14 15:16 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J14 15:16 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |
| PA19 | DEBUGSS | SWDIO | N/A | <ul><li>PA19 is used by SWD during debugging<br><ul><li>`J14 13:14 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J14 13:14 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0L2228 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0L2228 web page](https://www.ti.com/tool/LP-MSPM0L2228).

## Example Usage
Compile, load and run the example. LED toggles every 500 ms.
Measure the digital output of CLK_OUT.
