## Example Summary

The following example configures ADC0 to trigger a sequence of conversions on 4 different channels and stores the results in 4 different buffer arrays.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| ADC0 | PA25 | ADC12 Channel 2 Pin |
| ADC0 | PA27 | ADC12 Channel 0 Pin |
| ADC0 | PA22 | ADC12 Channel 7 Pin |
| ADC0 | PA24 | ADC12 Channel 3 Pin |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA25 | ADC0 | A2 | J1_2 | N/A |
| PA27 | ADC0 | A0 | J1_8 | J17 OFF Disconnect from Light Sensor D1 |
| PA22 | ADC0 | A7 | J3_24 | J16 ON/OFF: Connect/Disconnect from Light sensor D1<br>R83/C52 can be soldered for RC filter |
| PA24 | ADC0 | A3 | J3_27 | R80/C50 can be soldered for RC filter |
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

Compile, load and run the example. Connect vaild inputs ranging from 0-VCC to the ADC channels, floating pins are not guaranteed to read as 0.
