## Example Summary

The following example configures TimerA in PWM edge align mode and
outputs 4 PWM signal. The pair of signals on C0 and C1 are complementary
signals with deadband insertion. The pair of signals on C2 and C3 are also
complementary signals with dead band insertion.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| SYSCTL |  |  |  |  |
| TIMA0 | Capture/Compare Pin 0 | PA0 | J1_10 |  |
| TIMA0 | Capture/Compare Complementary Pin 0 | PA22 | J3_24 |  |
| TIMA0 | Capture/Compare Pin 1 | PA1 | J1_9 |  |
| TIMA0 | Capture/Compare Complementary Pin 1 | PA25 | J1_2 |  |
| EVENT |  |  |  |  |
| BOARD | Debug Clock | PA20 | J22_16 |  |
| BOARD | Debug Data In Out | PA19 | J22_14 |  |


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
Compile, load and run the example. Monitor the PWM signals on PA0, PA22, PB20,
and PB13.
The pair of signals on PA0 and PA22 are complementary signals with deadband
insertion.
The second pair of signals on PB20 and PB13 are also complementary signals with
deadband insertion.