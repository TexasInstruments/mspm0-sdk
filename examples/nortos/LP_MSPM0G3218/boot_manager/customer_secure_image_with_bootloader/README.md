## Example Summary

This is the a sample application image with bootloader designed to be used with the ```customer_secure_code``` example. It contains additional post-build steps in order to sign and encrypt the image with a provided test key and run the image from the boot application in either the primary or secondary slot.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOA | Standard with Wake Input with internal pull-down | PA18 | J3_26 |  |
| GPIOA | Standard with Wake Input with internal pull-down | PA18 | S1 | Populate Jumper(s): J8[1:2] |
| GPIOB | Standard Output | PB22 | LED2 Blue | Populate Jumper(s): J5[1:2] |
| GPIOB | Standard Output | PB26 | LED2 Red | Populate Jumper(s): J6[1:2] |
| GPIOB | Standard Output | PB27 | LED2 Green | Populate Jumper(s): J7[1:2] |
| UC0 | RX Pin | PA11 | Back-Channel UART RXD | Populate Jumper(s): J22[2:1] |
| UC0 | TX Pin | PA10 | Back-Channel UART TXD | Populate Jumper(s): J21[2:1] |
| SYSCTL |  |  |  |  |
| EVENT |  |  |  |  |
| BOARD | Debug Clock | PA20 | J101_16 |  |
| BOARD | Debug Data In Out | PA19 | J101_14 |  |

### Device Migration Recommendations
This project was developed for a superset device included in the MSPM0 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0x21x family devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the LP-MSPM0G3218 User's Guide.

## Example Usage
Follow the prerequisites in the ```docs/*/middleware/boot_manager```. Then, build the example in each configuration. This can be done by right-clicking on the project, then going to **Build Configurations > Build All** Then, load the image into the corresponding slot, optionally using the memory browser when in a debug session of the ```boot_application``` companion example. This process is discussed in more detail in the BIM User's Guide in the SDK middleware documentation.

By default, the primary image configuration will load and will flash the green LED using a timer interrupt, and the secondary image configuration will flash the blue LED.

Optionally, the images in the ```prebuilt_images``` can be used and loaded for quick verification and not using the prerequisites.
