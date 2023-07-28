## Example Summary

The following example configures fault detection feature on TimerA.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| TIMA1 | PB20 | Fault Pin 1 |
| SYSCTL | PA31 | CLK OUT |
| TIMA1 | PB4 | Counter Compare Pin 0 |
| TIMA1 | PB1 | Counter Compare Pin 1 |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PB20 | TIMA1 | FAULT1 | J4_36 | N/A |
| PA31 | SYSCTL | CLK_OUT | J4_37 | N/A |
| PB4 | TIMA1 | CCP0 | J4_40 | N/A |
| PB1 | TIMA1 | CCP1 | J4_39 | N/A |
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

- TIMA1_C0 is set to high state
- TIMA1_C1 is set to low state


Once the fault condition is cleared (i.e. fault pin remains in low state for at
least 5 timer clocks). TimerA will resume operation as follows and will perform
all three operations listed below simultaneously:

- TIMA1_C0 is set to low state
- TIMA1_C1 is set to high state
- Timer counter resumes operation based on counter value after enable. For
  this example, the counter starts counting down from the pwm period value.

The example configures CLK_OUT pin to output LFCLK to monitor the reaction time
of TimerA modules relative to fault condition generation.
