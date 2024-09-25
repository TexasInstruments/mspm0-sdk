## Example Summary
This example shows a basic example of an application calling a pre-initialization
boot hook function.
This allows an application to execute some instructions immediately after reset
or to reduce boot time by bypassing cinit.
The example jumps to main without using cinit to initialize global variables. As
a result, the memory structure gByteArray retains its value from the previous
SRAM state.
Note that the values will be random after power-up and SRAM should be
initialized by the application.

This example doesn't support GCC.

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
Compile, load and run the example. The LED should be off and USE_TEST pin set
at the beginning of the program execution and will turn on at the end of the
example. Note that gByteArray will not be zero when starting main. The values
will be random when running for the first time, or retained when restarting the
application.
Optionally, modify the code to return a non-zero value in pre-initialization
routine to initialize all .bss to zero.