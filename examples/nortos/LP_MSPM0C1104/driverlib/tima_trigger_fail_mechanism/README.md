## Example Summary

The following example configures fault detection feature on TimerA.

**Note**: Example requires Rev E3 or later of the MSPM0C1104 LaunchPad.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA22 | Standard Output |
| GPIOA | PA11 | Standard Output |
| TIMERFAULT0 | PA25 | Fault Pin 1 |
| SYSCTL | PA18 | CLK OUT |
| TIMA0 | PA2 | Capture/Compare Pin 0 |
| TIMA0 | PA6 | Capture/Compare Pin 1 |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0C1104](https://www.ti.com/tool/LP-MSPM0C1104) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA22 | GPIOA | PA22 | J1_8 | N/A |
| PA11 | GPIOA | PA11 | J1_9 | <ul><li>PA11 can be connected to an external 3.3V pull-up<br><ul><li>`J6 OFF` Disconnect 3.3V pull-up<br><li>`J6 ON` Connect 3.3V pull-up</ul></ul> |
| PA25 | TIMERFAULT0 | FAULT2 | J1_2 | N/A |
| PA18 | SYSCTL | CLK_OUT | J2_15 | N/A |
| PA2 | TIMA0 | CCP0 | J2_13 | N/A |
| PA6 | TIMA0 | CCP1 | J1_7 | N/A |
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
MSPM0 LaunchPad, please visit the [LP-MSPM0C1104 User's Guide](https://www.ti.com/lit/slau908).

## Example Usage
Make sure external fault pin logic level is low before running example. Compile,
load and run the example. Monitor TIMA1_C0, TIMA1_C1, External fault pin and
CLK_OUT. Analyze fault reaction behavior described in "Application Design
Details" section.

## Application Design Details
TimerA is configured to generate edge align PWM signals on TIMA1_C0/TIMA1_C0 and
monitor fault conditions via an external fault pin.

A fault condition is detected when the fault pin state remains in high state for
more than 5 timer clocks. This examples enables the fault input filter to handle
glitches on the fault input and is configured for consecutive periods mode for
3 timer clocks. The fault pin input is passed by a synchronizer which can take
up to 2 additional timer clock to start detecting consecutive periods.

Once a fault is detected , TimerA is configured to set the PWM as follows:

- TimerA is configured to set the PWM as follows:
    - TIMA1_C0 is set to high state
    - TIMA1_C1 is set to low state
- USER_LED_1 and USER_TEST pins will toggle every time a fault is detected


Once the fault condition is cleared (i.e. fault pin remains in low state for at
least 5 timer clocks). TimerA will resume operation as follows and will perform
all three operations listed below simultaneously:

- TIMA1_C0 is set to low state
- TIMA1_C1 is set to high state
- Timer counter resumes operation based on counter value after enable. For
  this example, the counter starts counting down from the pwm period value.

The example configures CLK_OUT pin to output LFCLK to monitor the reaction time
of TimerA modules relative to fault condition generation.
