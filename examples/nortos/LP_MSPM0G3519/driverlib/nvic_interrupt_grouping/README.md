## Example Summary

The following example configures TIMG0, TIMG1, and WWDT0 to toggle three
different LEDs to demonstrate usage of NVIC group functionality.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | Pin | Launchpad Pin |
| --- | --- | --- | --- |
| GPIOB | Standard Output | PB22 | LED2 Blue |
| GPIOB | Standard Output | PB26 | LED2 Red |
| GPIOB | Standard Output | PB27 | LED2 Green |
| BOARD | Debug Clock | PA20 | J19_16 |
| BOARD | Debug Data In Out | PA19 | J19_14 |

### Device Migration Recommendations
This project was developed for a superset device included in the MSPM0 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the LP-MSPM0G3519 User's Guide.

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
