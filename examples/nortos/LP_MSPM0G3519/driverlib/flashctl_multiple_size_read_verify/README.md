## Example Summary

This example demonstrates the read verify command for different data sizes, and with and without ECC.

The read verify command may be used to read a flash location and compare the data to data which is preloaded into the CMDDATA registers of the flash controller. This example shows read verify operations on multiple data sizes (8/16/32/64-bit) as well as demonstrating read verify with hardware generated ECC code and manually provided ECC code. Successful read verify operations as well as expected failures are demonstrated.

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

Compile, load and run the example.
The flash operations will start automatically when the example is run.
The application will hit a breakpoint if there is a failure.
LED1 and USER_TEST_PIN will toggle after all flash operations are completed
successfully.
