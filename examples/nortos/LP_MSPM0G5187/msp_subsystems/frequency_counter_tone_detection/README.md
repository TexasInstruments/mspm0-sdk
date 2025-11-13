## Example Summary

The following example configures the on-board comparator, TimerG0, and TimerG4 capturing analog signals to periodically convert to digital signals. An LED is toggled to indicate whether the input signal is within the desired frequency range. This example configures timers in down counting mode. 

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOA | Standard Output | PA24 | LED2 Green | Populate Jumper(s): J13[2:1] |
| SYSCTL |  |  |  |  |
| TIMA0 |  |  |  |  |
| TIMG0 |  |  |  |  |
| COMP0 | COMP Output Pin | PA11 | J4_33 |  |
| COMP0 | COMP Negative Channel 0 Pin | PB22 | J1_6 |  |
| EVENT |  |  |  |  |
| BOARD | Debug Clock | PA20 | J101_16 |  |
| BOARD | Debug Data In Out | PA19 | J101_14 |  |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0L1306 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the LP-MSPM0G5187 User's Guide.

## Example Usage
Connect input signal to PB22 (COMP_0 IN0-). Configure capture period and/or frequency range settings in `main.c`. Within sysconfig, the period between captures can be adjusted (TIMER > Desired Timer Period) as well as offset voltage (COMP > Input Code Through DACCODE0). Compile, load and run the example. LED2 toggles ON when the frequency is within desired range and toggles OFF when the frequency is outside of the desired range.

