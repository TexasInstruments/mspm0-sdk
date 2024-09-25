## Example Summary

The following example configures TimerA in PWM edge align mode and
outputs 4 PWM signal. The pair of signals on C0 and C1 are complementary
signals with deadband insertion. The pair of signals on C2 and C3 are also
complementary signals with dead band insertion.

**Note**: Example requires Rev E3 or later of the MSPM0C1104 LaunchPad.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| TIMA0 | PA2 | Counter Compare Pin 0 |
| TIMA0 | PA17 | Counter Compare Complementary Pin 0 |
| TIMA0 | PA6 | Counter Compare Pin 1 |
| TIMA0 | PA16 | Counter Compare Complementary Pin 1 |
| ADC0 | PA27 | ADC12 Channel 0 Pin |
| EVENT |  |  |
| DMA |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0C1104](https://www.ti.com/tool/LP-MSPM0C1104) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA2 | TIMA0 | CCP0 | J2_13 | N/A |
| PA17 | TIMA0 | CCP0_CMPL | J2_18 | N/A |
| PA6 | TIMA0 | CCP1 | J1_7 | N/A |
| PA16 | TIMA0 | CCP1_CMPL | J2_19 | <ul><li>PA16 is connected to S2 button to GND with no external pull resistor<br><ul><li>Press `S2` button to connect pin to GND<br><li>Don't use `S2` button if not needed by application</ul></ul> |
| PA27 | ADC0 | A0 | J1_4 | <ul><li>PA27 can be connected to XDS-110 backchannel<br><ul><li>`J101 7:8 OFF` Disconnect XDS-110 backchannel UART TX</ul></ul> |
| PA20 | DEBUGSS | SWCLK | J2_11 | <ul><li>PA20 is used by SWD during debugging<br><ul><li>`J101 13:14 ON` Connect to XDS-110 SWCLK while debugging<br><li>`J101 13:14 OFF` Disconnect from XDS-110 SWCLK if using pin in application</ul></ul> |
| PA19 | DEBUGSS | SWDIO | J2_17 | <ul><li>PA19 is used by SWD during debugging<br><ul><li>`J101 11:12 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J101 11:12 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0C1104 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0C1104 web page](https://www.ti.com/tool/LP-MSPM0C1104).

## Example Usage
Compile, load and run the example. Monitor the PWM signals on PA2, PA17, PA6,
and PA16.
The pair of signals on PA2 and PA17 are complementary signals with deadband
insertion.
The second pair of signals on PA6 and PA16 are also complementary signals with
deadband insertion.
