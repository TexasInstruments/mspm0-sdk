## Example Summary
This example uses parity SRAM by default.
The linker file is modified to use parity SRAM and memory is initialized before being used.
Accessing parity SRAM without initialization can result in a parity fault.

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
To observe a parity fault caused by parity SRAM, modify SRAM_PARITY_INITIALIZE to 0.
Compile, load and run the example.
If a fault is detected, the device will jump to NMI.

To initialize RAM, restore SRAM_PARITY_INITIALIZE to 1.
Compile, load and run the example.
The device should be able to access SRAM and hit the software breakpoint; however, a fault will trigger after overwriting the parity code and reading the same location.
