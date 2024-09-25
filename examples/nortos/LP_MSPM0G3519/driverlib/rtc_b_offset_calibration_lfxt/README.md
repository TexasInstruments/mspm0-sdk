## Example Summary

This example can be used to correct the crystal offset error.
Button are used to increase or decrease offset while an external tool measures
frequency using RTCCLK output.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA18 | Standard with Wake Input |
| GPIOB | PB21 | Standard Input with internal pull-up |
| SYSCTL | PA3 | LFXIN |
| SYSCTL | PA4 | LFXOUT |
| EVENT |  |  |
| DMA |  |  |
| RTC | PA9 | RTC Out Pin |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA18 | GPIOA | PA18 | J3_26/J3_29 | <ul><li>PA18 can be connected to S1 button to VCC with external pull-down<br><ul><li>`J8 ON` Connect S1 button and external pull-down<br><li>`J8 OFF` Disconnect S1 button and external pull-down</ul><br><li>PA18 can be connected to XDS-110 BSL_Invoke<br><ul><li>`J101 17:18 OFF` Disconnect pin to XDS-110 BSL Invoke</ul><br><li>PA18 can be connected to boosterpack pin J3_29<br><ul><li>`J15 2:3` Connect pin to J3_29<br><li>`J15 1:2/OFF` Disconnect pin from J3_29</ul><br><li>PA18 can be connected to boosterpack pin J3_26<br><ul><li>`R58` is soldered and connects pin to J3_26 by default</ul></ul> |
| PB21 | GPIOB | PB21 | J27_4 | <ul><li>PB21 is connected to S2 button to GND with no external pull resistor<br><ul><li>Press `S2` button to connect pin to GND<br><li>Don't use `S2` button if not needed by application</ul></ul> |
| PA3 | SYSCTL | LFXIN | J28_2 | <ul><li>PA3 is used for LFXT functionality by default in Launchpad<br><ul><li>`Y1` crystal is soldered and connected by default</ul><br><li>PA3 can be connected to J28_2<br><ul><li>`R10` is not soldered soldered by default<br><li>Solder `R10` to connect pin to J28_2</ul> |
| PA4 | SYSCTL | LFXOUT | J28_3 | <ul><li>PA4 is used for LFXT functionality by default in Launchpad<br><ul><li>`Y1` crystal is soldered and connected by default</ul><br><li>PA4 can be connected to J28_3<br><ul><li>`R11` is not soldered soldered by default<br><li>Solder `R11` to connect pin to J28_3</ul> |
| PA9 | RTC | RTC_OUT | J1_3 | <ul><li>PA9 can be connected to J1_3<br><ul><li>`J1 2:3` Connect pin to J1_3<br><li>`J1 1:2/OFF` Disconnect pin from J1_3</ul> |
| PA20 | DEBUGSS | SWCLK | N/A | <ul><li>PA20 is used by SWD during debugging<br><ul><li>`J101 15:16 ON` Connect to XDS-110 SWCLK while debugging<br><li>`J101 15:16 OFF` Disconnect from XDS-110 SWCLK if using pin in application</ul></ul> |
| PA19 | DEBUGSS | SWDIO | N/A | <ul><li>PA19 is used by SWD during debugging<br><ul><li>`J101 13:14 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J101 13:14 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0G3507 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0G3507 User's Guide](https://www.ti.com/lit/slau873).

## Example Usage
Compile, load and run the example.
The output frequency for offset calibration is by default set to 512Hz.
Connect external equipment such as oscilloscope to RTC_OUT.
Press S1 to increment offset by +1PPM, S2 to decrement correction by -1PPM.
