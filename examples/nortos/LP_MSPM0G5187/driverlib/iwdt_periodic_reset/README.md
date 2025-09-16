## Example Summary

Configures the IWDT module within the LFSS IP with a 500 ms period to be periodically reset by a
repeating timer interrupt.

This IWDT has its own system independent power and clock source. If the application software does not successfully reset the watchdog within
the programmed time, the watchdog generates a POR reset to the SoC.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOB | Standard Output | PB13 | LED2 Blue | Populate Jumper(s): J11[2:1] |
| GPIOB | Standard Output | PB14 | J3_23 |  |
| SYSCTL |  |  |  |  |
| TIMA0 |  |  |  |  |
| IWDT |  |  |  |  |
| EVENT |  |  |  |  |
| BOARD | Debug Clock | PA20 | J101_16 |  |
| BOARD | Debug Data In Out | PA19 | J101_14 |  |

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
MSPM0 LaunchPad, please visit the [LP-MSPM0G5187 User's Guide](https://www.ti.com/lit/slau967).

## Example Usage

Compile, load and run the example.

USER_LED will toggle every time the IWDT is reset successfully.
USER_TEST_PIN will mimic this behavior.
