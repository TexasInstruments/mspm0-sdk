## Example Summary

The following example configures TimerG4 to output a single PWM signal and update
the signal’s frequency and duty cycle by using the shadow load and shadow capture
compare feature. Shadow load will only update the timer load values on a zero event
and the shadow capture compare value (which controls the PWM duty cycle) update
is configured to update when the timer reaches a count of zero.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOA | Open-Drain Output | PA0 | Red LED1 | Populate Jumper(s): J4[1:2] |
| GPIOA | Standard Output | PA15 | J4_38 |  |
| SYSCTL |  |  |  |  |
| TIMA0 | Capture/Compare Pin 0 | PA18 | S1 | Populate Jumper(s): J3[1:2] |
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
Compile, load and run the example.

LED_1 will be off at the beginning of the program execution. LED_1 will turn on
once the PWM signal starts to be generated.
USER_TEST_PIN GPIO will mimic the behavior of the LED pin on the
BoosterPack header and can be used to verify the LED behavior.

PWM signal on PA18.
