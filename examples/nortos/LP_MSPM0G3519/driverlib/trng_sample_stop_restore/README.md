## Example Summary

The following examples configures the TRNG, then goes into STOP mode. The device
stays in STOP mode for 1 second and then is woken up by the Timer. The device
restores the configuration of the TRNG, and then the TRNG fills a buffer with
random data.

Some MSPM0G peripherals residing in PD1 domain don't retain register contents when
entering STOP or STANDBY modes, refer to the datasheet for more details.
TRNG is a peripheral in PD1 which exhibits this behavior.
Developers can decide to re-initialize the peripheral using the default
initialization (i.e. SYSCFG_DL_TRNG_init) from SysConfig. This approach
is more memory-efficient.
Alternatively, this example demonstrates using DriverLib APIs to save and
restore the register configuration of the peripheral before and after entering
low-power mode. This approach is recommended if the TRNG configuration is
modified at runtime.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | Pin | Launchpad Pin |
| --- | --- | --- | --- |
| GPIOA | Open-Drain Output | PA0 | Red LED1 |
| GPIOA | Standard Output | PA15 | J3_30 |
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
Compile, load and run the example. The LED (USER_LED_1) and test pins
(USER_TEST_PIN) will be set. LED will be off. After configuring the peripherals,
the application will go into STOP mode.
In 1 second, the timer interrupt will fire and wake up the device, and restore
the SW configuration of the TRNG. The TRNG will fill gTRNGBuffer with random
data.
In the case of an error with saving or restoring the configuration, the LED and
test pins will be set, and the LED will remain off. Otherwise, the LED and test
pins will be cleared, and the LED will turn on.
The application will hit a SW breakpoint to allow the user to inspect the
contents of gTRNGBuffer, and gIsRetentionError should be false.
