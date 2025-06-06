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

### Device Migration Recommendations
This project was developed for a superset device included in the LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the LP-MSPM0C1106 User's Guide.

## Example Usage
Compile, load and run the example. LED toggles every 500 ms.
Measure the digital output of CLK_OUT.
