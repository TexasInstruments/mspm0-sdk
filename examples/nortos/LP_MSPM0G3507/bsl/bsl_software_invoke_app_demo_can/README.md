## Example Summary

This example demonstrates an application request to reset the device and jump to
the ROM bootloader (BSL).

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA21 | Standard Input |
| GPIOB | PB22 | Standard Output |
| GPIOB | PB27 | Standard Output |
| SYSCTL | PA5 | HFXIN |
| SYSCTL | PA6 | HFXOUT |
| EVENT |  |  |
| CANFD0 | PA13 | RX Pin |
| CANFD0 | PA12 | TX Pin |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA21 | GPIOA | PA21 | J28_8 | <ul><li>PA21 can be connected to GND when used for VREF-<br><ul><li>`R3` is soldered by default connecting pin to GND<br><li>Remove `R3` to use pin for other functions</ul><br><li>PA21 can be connected to boosterpack<br><ul><li>`R10` is not soldered by default<br><li>Solder `R10` to connect pin to `J28_8`</ul></ul> |
| PB22 | GPIOB | PB22 | J27_5 | <ul><li>PB22 can be connected to LED2 Blue<br><ul><li>`J5 ON` Connect to LED2 Blue<br><li>`J15 OFF` Disconnect from LED2 Blue</ul></ul> |
| PB27 | GPIOB | PB27 | J27_10 | <ul><li>PB27 can be connected to LED2 Green<br><ul><li>`J7 ON` Connect to LED2 Green<br><li>`J7 OFF` Disconnect from LED2 Green</ul></ul> |
| PA5 | SYSCTL | HFXIN | J28_4 | <ul><li>PA5 is used for HFXT functionality by default in Launchpad<br><ul><li>`Y2` crystal is soldered and connected by default</ul><br><li>PA5 can be connected to J28_4<br><ul><li>`R15` is not soldered soldered by default<br><li>Solder `R15` to connect pin to J28_4</ul> |
| PA6 | SYSCTL | HFXOUT | J28_5 | <ul><li>PA6 is used for HFXT functionality by default in Launchpad<br><ul><li>`Y2` crystal is soldered and connected by default</ul><br><li>PA6 can be connected to J28_5<br><ul><li>`R17` is not soldered soldered by default<br><li>Solder `R17` to connect pin to J28_5</ul> |
| PA13 | CANFD0 | CANRX | J4_32/J26_3 | <ul><li>PA13 can be connected to CAN/LIN connector in addition to boosterpack connector:<br><ul><li>To use on J26 CAN/LIN connector:<br>  `R65` is populated by default and connects pin to `J26_3`</ul></ul> |
| PA12 | CANFD0 | CANTX | J4_32/J26_1 | <ul><li>PA12 can be connected to CAN/LIN connector in addition to boosterpack connector:<br><ul><li>To use on J26 CAN/LIN connector:<br>  `R64` is populated by default and connects pin to `J26_1`</ul></ul> |
| PA20 | DEBUGSS | SWCLK | N/A | <ul><li>PA20 is used by SWD during debugging<br><ul><li>`J101 15:16 ON` Connect to XDS-110 SWCLK while debugging<br><li>`J101 15:16 OFF` Disconnect from XDS-110 SWCLK if using pin in application</ul></ul> |
| PA19 | DEBUGSS | SWDIO | N/A | <ul><li>PA19 is used by SWD during debugging<br><ul><li>`J101 13:14 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J101 13:14 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0G3507 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#manual-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0G3507 User's Guide](https://www.ti.com/lit/slau873).

## Example Usage

Connect a proper BSL Host to the device. Download interface plugin or secondary BSL if used and then download this demo into the device.
(It will not erase the interface plugin or secondary BSL code due to they are static write protected)

The example will begin toggling LED2.Blue while waiting for the user to press
the S2 button or send 0x22 from host trigger BSL.

When S2 is pressed or 0x22 received, the application will clear LED2.Blue and light LED2.Green
for one second to indicate that the device is about to reset. After one second,
the device will reset with BOOTLOADERENTRY reset level. This will run the
bootcode, which will check if there is a request from the application, and then
invoke the BSL. At this point, both LED2.Blue and LED2.Green will be off.
Once the BSL has been invoked, the user can send commands to the BSL through the
external BSL Host.

To return to main, the user can trigger a POR/BOR on the device, or use
the BSL Host to send the application start command to the device.
