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

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA20 | DEBUGSS | SWCLK | N/A | J101 15:16 ON: Connect to XDS-110 SWCLK (debug) |
| PA19 | DEBUGSS | SWDIO | N/A | J101 13:14 ON: Connect to XDS-110 SWDIO (debug) |

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0G3507 User's Guide](https://www.ti.com/lit/slau846).

## Example Usage
Compile, load and run the example.
Note that gByteArray will not be zero when starting main. The values will be
random when running for the first time, or retained when restarting the
application.
Optionally, modify the code to return a non-zero value in pre-initialization
routine to initialize all .bss to zero.
