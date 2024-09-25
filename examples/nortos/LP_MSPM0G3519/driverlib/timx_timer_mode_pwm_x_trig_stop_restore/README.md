
## Example Summary
Two PWMs are generated for 1 second, then saves the PWM timer configuration
before going to STOP mode. The device stays in STOP mode for 1 second and then
restores PWM generation.

Some MSPM0G peripherals residing in Power Domain 1 (PD1) don't retain register contents when
entering STOP or STANDBY modes, refer to the datasheet for more details.
TimerA is a peripheral in PD1 which exhibits this behavior.
Developers can decide to re-initialize the peripheral using the default
initialization (i.e. SYSCFG_DL_TIMERA_INST_0_init) from SysConfig. This approach
is more memory-efficient.
Alternatively, this example demonstrates using DriverLib APIs to save and
restore the register configuration of the peripheral before and after entering
low-power mode. This approach is recommended if the Timer configuration is
modified at runtime.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | Pin | Launchpad Pin |
| --- | --- | --- | --- |
| GPIOA | Open-Drain Output | PA0 | Red LED1 |
| GPIOA | Standard Output with internal pull-down | PA15 | J3_30 |
| TIMA1 | Capture/Compare Pin 0 | PB4 | J4_36 |
| TIMA1 | Capture/Compare Pin 1 | PB3 | J4_40 |
| TIMA0 | Capture/Compare Pin 0 | PB8 | J2_15 |
| TIMA0 | Capture/Compare Pin 1 | PB12 | J2_13 |
| BOARD | Debug Clock | PA20 | J19_16 |
| BOARD | Debug Data In Out | PA19 | J19_14 |

### Device Migration Recommendations
This project was developed for a superset device included in the MSPM0 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the LP-MSPM0G3519 User's Guide.

## Example Usage
Compile, load and run the example.
Monitor PWM generation before and after STOP mode.
USER_TEST_PIN will go high when 1 sec timeout counter is started and go low
when the 1 sec period has completed. USER_LED_PIN will go low to turn on a LED
once the example finished running without an error.
