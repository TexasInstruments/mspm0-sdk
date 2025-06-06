## Example Summary
This example uses parity SRAM by default.
The linker file is modified to use parity SRAM and memory is initialized before being used.
Accessing parity SRAM without initialization can result in a parity fault.

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
To observe a parity fault caused by parity SRAM, modify SRAM_PARITY_INITIALIZE to 0.
Compile, load and run the example.
If a fault is detected, the device will jump to NMI.

To initialize RAM, restore SRAM_PARITY_INITIALIZE to 1.
Compile, load and run the example.
The device should be able to access SRAM and hit the software breakpoint;
however, a fault will trigger after overwriting the parity code and reading the
same location.

LED will turn on when expected NMI parity fault has been triggered.

LED will remain off if there is a problem during program flow.
USER_TEST_PIN GPIO will mimic the behavior of the LED pin on the BoosterPack
header and can be used to verify the LED behavior.
