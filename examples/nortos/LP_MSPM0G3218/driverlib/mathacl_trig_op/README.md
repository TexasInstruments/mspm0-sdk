## Example Summary
The following examples demonstrates the MATHACL trig operations.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOA | Open-Drain Output | PA0 | Red LED1 | Populate Jumper(s): J4[1:2] |
| GPIOA | Standard Output | PA15 | J3_30 |  |
| SYSCTL |  |  |  |  |
| EVENT |  |  |  |  |
| MATHACL |  |  |  |  |
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
MSPM0 LaunchPad, please visit the LP-MSPM0G3218 User's Guide.

## Example Usage
Compile, load and run the example.
The application will stop at the break point to allow the user to verify the
three sin/cos operations.
The value of op1Cos should be 1 (0x7FFFFFFF).
The value of op1Sin should be 0.
The value of op2Cos should be 0.707 (0x5A89E374).
The value of op2Sin should be 0.707 (0x5A7B0D41).
The value of op3Cos should be 0.
The value of op3Sin should be 1 (0x7FFFFFFF).
After the values have been calculated, LED1 will turn on to signal example is
complete. USER_TEST_PIN GPIO will mimic the behavior of the LED pin on the
BoosterPack header and can be used to verify the LED behavior.
