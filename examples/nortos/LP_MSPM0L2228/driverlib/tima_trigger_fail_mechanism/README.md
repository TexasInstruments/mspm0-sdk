## Example Summary

The following example configures fault detection feature on TimerA.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA0 | Open-Drain Output |
| GPIOA | PA21 | Standard Output |
| TIMERFAULT0 | PA26 | Fault Pin 0 |
| SYSCTL |  |  |
| TIMA0 | PB3 | Capture/Compare Pin 0 |
| TIMA0 | PB4 | Capture/Compare Pin 1 |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L2228](https://www.ti.com/tool/LP-MSPM0L2228) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA0 | GPIOA | PA0 | J3_10 | <ul><li>PA0 is 5V tolerant open-drain so it requires pull-up<br><ul><li>`J12 1:2` Use 3.3V pull-up<br><li>`J12 2:3` Use 5V pull-up</ul><br><li>PA0 can be connected to LED3<br><ul><li>`J2 ON` Connect to LED3<br><li>`J2 OFF` Disconnect from LED3</ul></ul> |
| PA21 | GPIOA | PA21 | J3_8 | <ul><li>PA21 can be connected to GND when used for VREF-<br><ul><li>`R4` is not soldered by default allowing the pin to be used for other functions</ul></ul> |
| PA26 | TIMERFAULT0 | FAULT0 | J7_32 | N/A |
| PB3 | TIMA0 | CCP0 | J7_40 | N/A |
| PB4 | TIMA0 | CCP1 | J7_11 | N/A |
| PA20 | DEBUGSS | SWCLK | N/A | <ul><li>PA20 is used by SWD during debugging<br><ul><li>`J14 15:16 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J14 15:16 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |
| PA19 | DEBUGSS | SWDIO | N/A | <ul><li>PA19 is used by SWD during debugging<br><ul><li>`J14 13:14 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J14 13:14 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0L2228 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0L2228 web page](https://www.ti.com/tool/LP-MSPM0L2228).

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

Once a fault is detected, the following actions occurr:

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
