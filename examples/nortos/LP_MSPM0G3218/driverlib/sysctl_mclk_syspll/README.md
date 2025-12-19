## Example Summary

This example configures MCLK frequency to 80 MHz by
using the SYSPLL to convert SYSOSC from 32 MHz to 80 MHz.
SYSPLLCLK2X is passed to MCLK and configured as its source.
Since MCLK isn't mapped to CLK_OUT pin, SYSPLLCLK1 will be
routed to CLK_OUT instead and is configured to be half the
speed of SYSPLLCLK2X. Thus, CLK_OUT will have a 40MHz signal
that can be measured.

TimerG is configured in periodic mode and toggles an LED every 500 ms.

Not valid on all devices that do not support up to 80MHz.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOA | Open-Drain Output | PA0 | Red LED1 | Populate Jumper(s): J4[1:2] |
| TIMA0 |  |  |  |  |
| EVENT |  |  |  |  |
| BOARD | Debug Clock | PA20 | J101_16 |  |
| BOARD | Debug Data In Out | PA19 | J101_14 |  |

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
MSPM0 LaunchPad, please visit the LP-MSPM0G3218 User's Guide.

## Example Usage
Compile, load and run the example. LED toggles every 500 ms.
Connect CLK_OUT to a oscilloscope or a logic analyzer to measure the output frequency at 40MHz.
