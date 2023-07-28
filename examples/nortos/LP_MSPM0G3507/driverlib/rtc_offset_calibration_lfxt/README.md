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
| RTC | PA9 | RTC Out Pin |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA18 | GPIOA | PA18 | J3_26/J3_29 | J15 2:3 Connect to J3_29<br>R58 connects to J3_26 (default)<br>R23 connects to J101 and J8 for BSL_Invoke (default)<br>J8 ON/OFF Connect/Disconnect S1 for BSL_Invoke (with pull-down)<br>J101 17:18 Connect/Disconnect from XDS-110 BSL_Invoke<br>R66/R67 for thermistor RT1 not soldered by default |
| PB21 | GPIOB | PB21 | J27_4 | Connected to S2 button (no pull resistor) |
| PA3 | SYSCTL | LFXIN | J28_2 | Connected to LFXT Y1<br>Solder R10 to connect to J28_2 |
| PA4 | SYSCTL | LFXOUT | J28_3 | Connected to LFXT Y1<br>Solder R11 to connect to J28_3 |
| PA9 | RTC | RTC_OUT | J1_3 | J1 2:3 Connect to J1_3 |
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
The output frequency for offset calibration is by default set to 512Hz.
Connect external equipment such as oscilloscope to RTC_OUT.
Press S1 to increment offset by +1PPM, S2 to decrement correction by -1PPM.
