## Example Summary

The following example configures WWDT0 in watchdog mode with a period of
500ms. Reset window 0 is set to be closed for 25% of the total period (125ms).
TimerG is configured to reset the watchdog timer periodically.
The Timer period is set to 300ms. It will successfully restart the WWDT counter
within the open window.

LFCLK is sourced from the internal low frequency oscillator (LFOSC).
The device remains in STANDBY mode while waiting for an interrupt.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOA | Standard Output | PA0 | Red LED1 | Populate Jumper(s): J4[1:2] |
| GPIOA | Standard Output | PA15 | J4_38 |  |
| SYSCTL |  |  |  |  |
| TIMG1 |  |  |  |  |
| EVENT |  |  |  |  |
| WWDT0 |  |  |  |  |
| BOARD | Debug Clock | PA20 | J13_4 | Populate Component(s): J13 |
| BOARD | Debug Data In Out | PA19 | J13_2 | Populate Component(s): J13 |

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
MSPM0 LaunchPad, please visit the LP-MSPM0H3216 User's Guide.

## Example Usage
Compile, load and run the example.
The LED and USER_TEST pin will toggle at every interval of 300ms.
