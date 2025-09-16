## Example Summary

The following example configures TimerG in one shot mode and
turns off an LED after 500 ms. The device remains in STANDBY mode while waiting
for an interrupt.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOA | Open-Drain Output | PA0 | Red LED1 | Populate Jumper(s): J2[1:2] |
| GPIOA | Standard Output | PA15 | J3_25 | |
| SYSCTL |  |  |  |  |
| TIMG1 |  |  |  |  |
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
Compile, load and run the example. LED1 turns off after 500 ms and device will
remain in STANDBY mode.
USER_TEST_PIN GPIO will mimic the behavior of the LED pin on the
BoosterPack header and can be used to verify the LED behavior.
