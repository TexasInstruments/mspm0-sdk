## Example Summary

This example will provide a beginning beta implementation of the boot image manger. It is comprised of all the necessary files and procedures in order to run application images. The default public key is image_signTEST found in ```source/third_party/mcuboot/```. This is an example with a companion example ```bim_sample_image``` that will provide application images that work with this boot application.

It is highly recommended to read the users guide found in ```docs/*/middleware/boot_manager/Boot_Image_Manager_Users_guide.html``` prior to running the example.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOB | PB22 | Standard Output |
| GPIOB | PB26 | Standard Output |
| GPIOB | PB27 | Standard Output |
| SYSCTL |  |  |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PB22 | GPIOB | PB22 | J27_5 | J5 ON/OFF Connect/Disconnect LED2 Blue |
| PB26 | GPIOB | PB26 | J27_8 | J6 ON/OFF Connect/Disconnect LED2 Red |
| PB27 | GPIOB | PB27 | J27_10 | J7 ON/OFF Connect/Disconnect LED2 Green |
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
Compile, load and run the example. Load any application image, such as those produced by the corresponding example ```bim_sample_image```, either compiled and signed beforehand or using the pre-built images provided in the example in the ```prebuilt_images``` folder.

If successful, this example will jump to a valid application. If unsuccessful, the example will flash the red LED.

Further discussion of this example is covered in the BIM Users's Guide in the SDK middleware documentation.
