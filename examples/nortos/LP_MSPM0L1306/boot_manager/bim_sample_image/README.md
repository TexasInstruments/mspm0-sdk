## Example Summary

This is the a sample application image designed to be used with the ```boot_application``` example. It contains additional post-build steps in order to sign the image with a provided test key and run the image from the boot application in either the primary or secondary slot.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA27 | Standard Output |
| GPIOA | PA26 | Standard Output |
| GPIOA | PA13 | Standard Output |
| SYSCTL |  |  |
| TIMG0 |  |  |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L1306](https://www.ti.com/tool/LP-MSPM0L1306) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA27 | GPIOA | PA27 | J4_37 | J13 ON/OFF: Connect/Disconnect LED2 Blue |
| PA26 | GPIOA | PA26 | J4_38 | J12 ON/OFF: Connect/Disconnect LED2 Red |
| PA13 | GPIOA | PA13 | J4_39 | J3 ON/OFF: Connect/Disconnect LED2 Green |
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
Follow the prerequisites in the ```docs/*/middleware/boot_manager```. Then, build the example in each configuration. This can be done by right-clicking on the project, then going to **Build Configurations > Build All** Then, load the image into the corresponding slot, optionally using the memory browser when in a debug session of the ```boot_application``` companion example. This process is discussed in more detail in the BIM User's Guide in the SDK middleware documentation.

By default, the primary image configuration will load and will flash the green LED using a timer interrupt, and the secondary image configuration will flash the blue LED.

Optionally, the images in the ```prebuilt_images``` can be used and loaded for quick verification and not using the prerequisites.