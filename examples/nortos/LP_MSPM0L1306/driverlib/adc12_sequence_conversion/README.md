## Example Summary

The following example configures ADC0 to trigger sequence of conversions on 4 different channels and stores the results in 4 different buffer arrays.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| ADC0 | PA25 | ADC12 Channel 2 Pin |
| ADC0 | PA27 | ADC12 Channel 0 Pin |
| ADC0 | PA18 | ADC12 Channel 7 Pin |
| ADC0 | PA24 | ADC12 Channel 3 Pin |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L1306](https://www.ti.com/tool/LP-MSPM0L1306) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA25 | ADC0 | A2 | J1_2 | J4 ON/OFF: Connect/disconnect from photodiode D1<br>J1 2:3: Connect to Thermistor RT1<br>J15 ON: Connects Thermistor RT1, if needed |
| PA27 | ADC0 | A0 | J4_37 | J13 OFF: Disconnect LED2 Blue |
| PA18 | ADC0 | A7 | J3_26 | J11 OFF: Disconnect S1 button (with pull-down)<br>J101 17:18 OFF: Disconnect from XDS-110 BSL_Invoke |
| PA24 | ADC0 | A3 | J1_5 | J5 ON/OFF: Connect/disconnect from photodiode D1<br>R93/R94 can be soldered for thermistor RT1<br>J15 ON: Connects Thermistor RT1, if needed |
| PA20 | DEBUGSS | SWCLK | J2_13 | J101 15:16 ON: Connect to XDS-110 SWCLK (debug) |
| PA19 | DEBUGSS | SWDIO | J2_17 | J101 13:14 ON: Connect to XDS-110 SWDIO (debug) |

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0L1306 User's Guide](https://www.ti.com/lit/slau869).

## Example Usage

Compile, load and run the example. Connect vaild inputs ranging from 0-VCC to the ADC channels, floating pins are not guaranteed to read as 0.
