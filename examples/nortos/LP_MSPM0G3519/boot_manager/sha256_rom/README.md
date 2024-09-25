## Example Summary
Users can use the sha api in ROM open section instead of replicating the library in user space. 
This example is a demonstration of using SHA api of ROM open section to apply hash on SRAM data.

The example tries to calulcate the SHA of 4 bytes of data {0x00000000} present in SRAM.
The SHA 256 digest value can be fetched from the shaDigest variable after the program reaches while loop.

**Note**: Example Uses SHA 256 algorithm.

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3519](https://www.ti.com/tool/LP-MSPM0G3519) for LaunchPad information, including user guide and hardware files.

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0G3519 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#manual-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0G3519 User's Guide](https://www.ti.com/lit/slau908).

## Example Usage
Compile, load and run the example on device, the resultant hash can be read from calculated_hash variable. 
