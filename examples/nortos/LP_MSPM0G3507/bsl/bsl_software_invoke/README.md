## Example Summary

This example demonstrates an application request to reset the device and jump to
the ROM bootloader (BSL).

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOB | PB21 | Standard Input with internal pull-up |
| GPIOB | PB22 | Standard Output |
| GPIOB | PB27 | Standard Output |
| SYSCTL |  |  |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PB21 | GPIOB | PB21 | J27_4 | Connected to S2 button (no pull resistor) |
| PB22 | GPIOB | PB22 | J27_5 | J5 ON/OFF Connect/Disconnect LED2 Blue |
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
