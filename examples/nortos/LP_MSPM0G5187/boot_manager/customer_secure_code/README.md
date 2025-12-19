## Example Summary

This is a sample customer secure code showcasing several cybersecurity features of the device while retaining the ability to debug and evaluate/modify the example for a given application. It is designed to be used with either the ```customer_secure_image_with_bootloader``` or ```customer_secure_sample_image``` projects.

## Requirements

The ECC and SHA libaries used by this example are compiled with TI Clang 4.0.0 with Link Time Optimization (LTO). This is not compatible
with previous versions of TI Clang (3.X and older). Thus, the user must download and use the updated TI Clang toolchain from [the product page](https://www.ti.com/tool/download/ARM-CGT-CLANG/4.0.0.LTS) if not installed already.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOA | Standard with Wake Output | PA17 | LED2 Red | Populate Jumper(s): J12[2:1] |
| GPIOB | Standard Output | PB13 | LED2 Blue | Populate Jumper(s): J11[2:1] |
| BOARD | Debug Clock | PA20 | J101_16 |  |
| BOARD | Debug Data In Out | PA19 | J101_14 |  |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0L2228 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#manual-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0G5187 web page](https://www.ti.com/tool/LP-MSPM0G5187).

## Example Usage

Follow the prerequisites and guide found in ```docs/*/middleware/boot_manager```. When debugging, make sure to load the second, identical CSC image into the far bank as well as described in the guide.
