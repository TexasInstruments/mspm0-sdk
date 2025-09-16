## Example Summary

This example demonstrates how to write to the BCR and BSL configuration structures that are in NONMAIN Memory.

The BCR and BSL configuration data structures are both contained within a single flash sector in the NONMAIN
flash memory region.

Both the BCR and BSL configuration data structures are protected by individual 16-bit CRCs to improve security.
The application first calculates the 16-bit CRCs for both structures based on the user configuration values, and then sets the calculated CRC for each structure.

To successfully change any parameter in the boot configuration, the application must erase the entire NONMAIN sector. Then both the BCR and BSL configuration structures are re-programed with the given user configuration values.

The BCR and BSL configuration structures at application startup are configured with the default values. To update the values, the user should refer to  boot_config.h to view the possible values that may be configured to BCR and BSL configuration structures.

*Note*: Please use caution before erasing and writing to NONMAIN memory.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOA | Open-Drain Output | PA0 | Red LED1 | Populate Jumper(s): J4[1:2] |
| SYSCTL |  |  |  |  |
| EVENT |  |  |  |  |
| CRC |  |  |  |  |
| BOARD | Debug Clock | PA20 | J14_4 |  |
| BOARD | Debug Data In Out | PA19 | J14_2 |  |

### Device Migration Recommendations
This project was developed for a superset device included in the LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#non-sysconfig-compatible-project-migration)
for information about migrating to other MSPM0 devices.

## Example Usage

Compile, load and run the example.
The flash operations will start automatically when the example is run.
LED0 will blink after all flash executions are completed.
The user can view the contents of memory to verify the results of the write operations.
