## Example Summary

This example demonstrates an application request to reset the device and jump to
the ROM bootloader (BSL).

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA5 | Standard Input with internal pull-up |
| GPIOB | PB16 | Standard Output |
| GPIOB | PB18 | Standard Output |
| SYSCTL |  |  |
| SPI0 | PB3 | SPI SCLK (Clock) |
| SPI0 | PB2 | SPI PICO (Peripheral In, Controller Out) |
| SPI0 | PA7 | SPI POCI (Peripheral Out, Controller In) |
| SPI0 | PA8 | SPI CS0 (Chip Select 0) |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L1117](https://www.ti.com/tool/LP-MSPM0L1117) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA5 | GPIOA | PA5 | J6_1 | <ul><li>PA5 is connected to S2 button to GND with no external pull resistor<br><ul><li>Press `S2` button to connect pin to GND<br><li>Don't use `S2` button if not needed by application</ul></ul> |
| PB16 | GPIOB | PB16 | J4_30 | <ul><li>PB16 can be connected to LED2 Blue<br></ul> |
| PB18 | GPIOB | PB18 | J4_31 | <ul><li>PB18 can be connected to LED2 Green<br></ul> |
| PB3 | SPI0 | SCLK | J1_7 | N/A |
| PB2 | SPI0 | PICO | J2_15 | N/A |
| PA7 | SPI0 | POCI | J2_19 | N/A |
| PA8 | SPI0 | CS0 | J1_4 | N/A |
| PA20 | DEBUGSS | SWCLK | N/A | <ul><li>PA20 is used by SWD during debugging<br><ul><li>`J101 15:16 ON` Connect to XDS-110 SWCLK while debugging<br><li>`J101 15:16 OFF` Disconnect from XDS-110 SWCLK if using pin in application</ul></ul> |
| PA19 | DEBUGSS | SWDIO | N/A | <ul><li>PA19 is used by SWD during debugging<br><ul><li>`J101 13:14 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J101 13:14 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0L1117 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#manual-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0L1117 User's Guide](https://www.ti.com/lit/slau953).

## Example Usage

Connect a proper BSL Host to the device. Download interface plugin or secondary BSL if used and then download this demo into the device.
(It will not erase the interface plugin or secondary BSL code due to they are static write protected)

The example will begin toggling LED4.Blue while waiting for the user to press
the S2 button or send 0x22 from host trigger BSL.

When S2 is pressed or 0x22 received, the application will clear LED4.Blue and light LED4.Green
for one second to indicate that the device is about to reset. After one second,
the device will reset with BOOTLOADERENTRY reset level. This will run the
bootcode, which will check if there is a request from the application, and then
invoke the BSL. At this point, both LED4.Blue and LED4.Green will be off.
Once the BSL has been invoked, the user can send commands to the BSL through the
external BSL Host.

To return to main, the user can trigger a POR/BOR on the device, or use
the BSL Host to send the application start command to the device.
