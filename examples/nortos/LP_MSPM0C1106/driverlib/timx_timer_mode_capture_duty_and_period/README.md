## Example Summary

The following example configures TimerG4 in combined capture mode (configure to
measure a PWM signal's period and duty cycle). TimerG1 is used to generate a the
PWM signal. This examples configures timers in down counting mode.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| SYSCTL |  |  |  |  |
| TIMG14 | Capture/Compare Pin 0 | PA10 | J2_15 |  |
| TIMG1 | Capture/Compare Pin 0 | PB2 | J4_31 |  |
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
Connect a jumper cable between PA10 (GPIO_TIMER_CAPTURE_C0_PIN) and PA26
(GPIO_PWM_C0_PIN).
Compile, load and run the example.
The application will capture two full periods before hitting a SW breakpoint.
This ensures that the timers are fully synchronized (please refer the TRM for
additional details). At the breakpoint, the user can inspect the values of
pwmPeriod and pwmDuty.
The value of pwmDuty should be set to ~50.
The value of pwmPeriod should be set to ~1000.
