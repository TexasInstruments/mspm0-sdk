## Example Summary

This example demonstrates how to write to the BCR and BSL configuration structures that are in NONMAIN Memory.

The BCR and BSL configuration data structures are both contained within a single flash sector in the NONMAIN
flash memory region.

Both the BCR and BSL configuration data structures are protected by individual 32-bit CRCs to improve security.
The application first calculates the 32-bit CRCs for both structures based on the user configuration values, and then sets the calculated CRC for each structure.

To successfully change any parameter in the boot configuration, the application must erase the entire NONMAIN seector. Then both the BCR and BSL configuration structures are re-programed with the given user configuration values.

The BCR and BSL configuration structures at application startup are configured with the default values. To update the values, the user should refer to  boot_config.h to view the possible values that may be configured to BCR and BSL configuration structures.

*Note*: Please use caution before erasing and writing to NONMAIN memory.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA0 | Open-Drain Output |
| SYSCTL |  |  |
| EVENT |  |  |
| CRC |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L1306](https://www.ti.com/tool/LP-MSPM0L1306) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA0 | GPIOA | PA0 | J1_10 | This pin is 5V tolerant open-drain and requires pull-up.<br>J2 ON/OFF: Connect/Disconnect LED1<br>J10 1:2 5V Pull-up<br>J10 2:3 3.3V Pull-up |
| PA20 | DEBUGSS | SWCLK | J2_13 | J101 15:16 ON: Connect to XDS-110 SWCLK (debug) |
| PA19 | DEBUGSS | SWDIO | J2_17 | J101 13:14 ON: Connect to XDS-110 SWDIO (debug) |

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0L1306 User's Guide](https://www.ti.com/lit/slau869).

## Example Usage

Compile, load and run the example.
The flash operations will start automatically when the example is run.
LED0 will blink after all flash executions are completed.
The user can view the contents of memory to verify the results of the write operations.
