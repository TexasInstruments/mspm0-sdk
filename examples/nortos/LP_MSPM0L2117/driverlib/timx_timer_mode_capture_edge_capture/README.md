## Example Summary

The following example configures the timer to measure the time from the start of
the capture operation to the signal edge.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOA | Open-Drain Output | PA0 | Red LED1 | Populate Jumper(s): J2[1:2] |
| GPIOA | Standard Output | PA15 | J3_25 | |
| TIMG2 | Capture/Compare Pin 0 | PB15 | J2_18 |  |
| SYSCTL |  |  |  |  |
| EVENT |  |  |  |  |
| BOARD | Debug Clock | PA20 | J14_15 |  |
| BOARD | Debug Data In Out | PA19 | J14_13 |  |

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
MSPM0 LaunchPad, please visit the LP-MSPM02117 User's Guide.

## Example Usage
Connect to PB15 (GPIO_TIMER_CAPTURE_C0_PIN) to an edge generator source (it can
also be connected to a momentary switch).
Compile, load and run the example.
Generate a rising edge on PB15 (GPIO_TIMER_CAPTURE_C0_PIN). This will cause the
application to hit the breakpoint instruction and the value of `edgeCapture` can
be inspected to determine the time when the edge occurred.

After the variable `edgeCapture` is inspected, the user can resume
execution and detect a new rising edge.

LED_1 will toggle every time a rising edge is detected and USER_TEST_PIN GPIO
will mimic the behavior of the LED pin on the BoosterPack header and can be used
to verify the LED behavior.
