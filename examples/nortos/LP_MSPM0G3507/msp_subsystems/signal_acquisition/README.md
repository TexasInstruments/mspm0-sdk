## Example Summary

This example demonstrates how the MSPM0 OPA and a single ADC channel can accommodate two different sensor input requirements. One requiring a unity-gain buffer, the other requiring a gain of x2.  The OPA output is internally connected to ADC0 channel 13 is also connected to PA22 for external monitoring.  During one measurement OPA0 is configured as a unity-gain buffer, while during the other measurement it is configured as a non-inverting amplifier with x2 gain.  
  
## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| ADC0 |  |  |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
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
Compile, load, and run the example.
OPA0 is configured first as a unity gain (1x) buffer amplifier using the non-inverting input (OPA0_IN0+) pin PA26.
Apply a voltage ranging from 0 to 3.3vdc to the input.  The ADC is configured to measure the OPA0 output and convert to a digital code ranging from 0 to 4095 counts. A SW breakpoint is set to see the ADC results.

Next, OPA0 is configured as a x2 non-inverting amplifier using the non-inverting input(OPA_IN1+) PA25.  Apply a voltage ranging from 0 to 1.65 vdc to the input.  Note, monitoring the OPA0 output voltage is possible using a digital multi-meter connected to PA22.
