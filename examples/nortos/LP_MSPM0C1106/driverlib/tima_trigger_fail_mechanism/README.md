## Example Summary

The following example configures fault detection feature on TimerA.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOA | Standard Output | PA26 | J1_8 |  |
| GPIOA | Standard Output | PA25 | J1_7 |  |
| TIMER FAULT 0 | Fault Pin 0 | PA8 | J2_13 |  |
| SYSCTL |  |  |  |  |
| TIMA0 | Capture/Compare Pin 0 | PA0 | J4_34 |  |
| TIMA0 | Capture/Compare Pin 1 | PA7 | J2_12 |  |
| EVENT |  |  |  |  |
| BOARD | Debug Clock | PA20 | J14_4 |  |
| BOARD | Debug Data In Out | PA19 | J14_2 |  |

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
