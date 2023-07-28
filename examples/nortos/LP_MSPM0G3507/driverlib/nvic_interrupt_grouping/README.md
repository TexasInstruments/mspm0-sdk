## Example Summary

The following example configures TIMG0, TIMG1, and WWDT0 to toggle three
different LEDs to demonstrate usage of NVIC group functionality.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOB | PB22 | Standard Output |
| GPIOB | PB26 | Standard Output |
| GPIOB | PB27 | Standard Output |
| SYSCTL |  |  |
| TIMG0 |  |  |
| TIMG6 |  |  |
| EVENT |  |  |
| WWDT0 |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PB22 | GPIOB | PB22 | J27_5 | J5 ON/OFF Connect/Disconnect LED2 Blue |
| PB26 | GPIOB | PB26 | J27_8 | J6 ON/OFF Connect/Disconnect LED2 Red |
| PB27 | GPIOB | PB27 | J27_10 | J7 ON/OFF Connect/Disconnect LED2 Green |
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
TIMG0, TIMG1, and WWDT0 have each been configured with a period of 500ms. Each
peripheral will toggle a different LED when its interrupt is triggered.
    - LED2.Blue will toggle when TIMG0 timer expires
    - LED2.Red will toggle when TIMG1 timer expires
    - LED2.Green will toggle when WWDT0 timer expires
Note: TIMG0 and TIMG1 are sourced by SYSOSC, and WWDT0 is sourced by LFCLK.
Since SYSOSC has a higher frequency deviation tolerance compared to LFCLK, the
LEDs toggled by TIMG0 and TIMG1 may change at a slightly different period
compared to the LED toggled by WWDT0.
