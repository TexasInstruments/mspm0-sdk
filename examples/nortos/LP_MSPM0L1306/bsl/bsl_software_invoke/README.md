## Example Summary

This example demonstrates an application request to reset the device and jump to
the ROM bootloader (BSL).

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA14 | Standard Input with internal pull-up |
| GPIOA | PA27 | Standard Output |
| GPIOA | PA13 | Standard Output |
| SYSCTL |  |  |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L1306](https://www.ti.com/tool/LP-MSPM0L1306) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA14 | GPIOA | PA14 | N/A | Connected to S2 button (no pull resistor) |
| PA27 | GPIOA | PA27 | J4_37 | J13 ON/OFF: Connect/Disconnect LED2 Blue |
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

Connect a proper BSL Host to the device. Please refer to the BSL User's Guide
for more details.

Compile, load and run the example.

The example will begin toggling LED2.Blue while waiting for the user to press
the S2 button.

When S2 is pressed, the application will clear LED2.Blue and light LED2.Green
for one second to indicate that the device is about to reset. After one second,
the device will reset with BOOTLOADERENTRY reset level. This will run the
bootcode, which will check if there is a request from the application, and then
invoke the BSL. At this point, both LED2.Blue and LED2.Green will be off.
Once the BSL has been invoked, the user can send commands to the BSL through the
external BSL Host.

To return to main, the user can trigger a POR/BOR on the device, or use
the BSL Host to send the application start command to the device.
