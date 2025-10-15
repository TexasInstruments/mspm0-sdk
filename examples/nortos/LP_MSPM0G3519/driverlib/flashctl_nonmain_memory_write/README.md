## Example Summary

This example demonstrates how to write to the BCR and BSL configuration structures that are in NONMAIN Memory.

The BCR and BSL configuration data structures are both contained within a single flash sector in the NONMAIN
flash memory region.

Both the BCR and BSL configuration data structures are protected by individual 32-bit CRCs to improve security.
The application first calculates the 32-bit CRCs for both structures based on the user configuration values, and then sets the calculated CRC for each structure.

To successfully change any parameter in the boot configuration, the application must erase the entire NONMAIN sector. Then both the BCR and BSL configuration structures are re-programed with the given user configuration values.

The BCR and BSL configuration structures at application startup are configured with the default values. To update the values, the user should refer to  boot_config.h to view the possible values that may be configured to BCR and BSL configuration structures.

*Note*: Please use caution before erasing and writing to NONMAIN memory.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOA | Open-Drain Output | PA0 | Red LED1 | Populate Jumper(s): J6[1:2] |
| SYSCTL |  |  |  |  |
| EVENT |  |  |  |  |
| CRCP |  |  |  |  |
| BOARD | Debug Clock | PA20 | J19_15 | Populate Jumper(s): J19[16:15] |
| BOARD | Debug Data In Out | PA19 | J19_13 | Populate Jumper(s): J19[14:13] |
## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3519](https://www.ti.com/tool/LP-MSPM0G3519) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA0 | GPIOA | PA0 | J27_9 | <ul><li>PA0 is 5V tolerant open-drain so it requires pull-up<br><ul><li>`J19 1:2` Use 3.3V pull-up<br><li>`J19 2:3` Use 5V pull-up</ul><br><li>PA0 can be connected to LED1<br><ul><li>`J4 ON` Connect to LED1<br><li>`J4 OFF` Disconnect from LED1</ul></ul> |
| PA20 | DEBUGSS | SWCLK | N/A | <ul><li>PA20 is used by SWD during debugging<br><ul><li>`J101 15:16 ON` Connect to XDS-110 SWCLK while debugging<br><li>`J101 15:16 OFF` Disconnect from XDS-110 SWCLK if using pin in application</ul></ul> |
| PA19 | DEBUGSS | SWDIO | N/A | <ul><li>PA19 is used by SWD during debugging<br><ul><li>`J101 13:14 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J101 13:14 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0G3519 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#non-sysconfig-compatible-project-migration)
for information about migrating to other MSPM0 devices.

## Example Usage

Compile, load and run the example.
The flash operations will start automatically when the example is run.
LED0 will blink after all flash executions are completed.
The user can view the contents of memory to verify the results of the write operations.
