## Example Summary

This demo shows how to program a MSPM0L1306 MCU using a LP-MSPM0L1306 as host by BSL SPI interface.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA14 | Standard Input with internal pull-up |
| GPIOA | PA0 | Open-Drain Output |
| GPIOA | PA27 | Standard Output |
| GPIOA | PA3 | Standard Output(To control NRST pin) |
| GPIOA | PA7 | Standard Output(To control invoke pin) |
| SYSCTL |  |  |
| SPI0 | PA6 | SPI SCLK (Clock) |
| SPI0 | PA5 | SPI PICO (Peripheral In, Controller Out) |
| SPI0 | PA4 | SPI POCI (Peripheral Out, Controller In) |
| SPI0 | PA8 | SPI CS0 (Chip Select 0) |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L1306](https://www.ti.com/tool/LP-MSPM0L1306) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA14 | GPIOA | PA14 | N/A | <ul><li>PA14 is connected to S2 button to GND with no external pull resistor<br><ul><li>Press `S2` button to connect pin to GND<br><li>Don't use `S2` button if not needed by application</ul></ul> |
| PA0 | GPIOA | PA0 | J1_10 | <ul><li>PA0 is 5V tolerant open-drain so it requires pull-up<br><ul><li>`J10 2:3` Use 3.3V pull-up<br><li>`J10 1:2` Use 5V pull-up</ul><br><li>PA0 can be connected to LED1<br><ul><li>`J2 ON` Connect to LED1<br><li>`J2 OFF` Disconnect from LED1</ul></ul> |
| PA27 | GPIOA | PA27 | J4_37 | <ul><li>PA27 can be connected to LED2 Blue<br><ul><li>`J13 ON` Connect to LED2 Blue<br><li>`J13 OFF` Disconnect from LED2 Blue</ul></ul> |
| PA3 | GPIOA | PA3 | J2_19 | N/A |
| PA7 | GPIOA | PA7 | J2_18 | N/A |
| PA6 | SPI0 | SCLK | J1_7 | N/A |
| PA5 | SPI0 | MOSI | J2_15 | N/A |
| PA4 | SPI0 | MISO | J2_14 | N/A |
| PA8 | SPI0 | CS0 | J1_4 | <ul><li>PA8 can be connected to XDS-110 backchannel or to boosterpack connector:<br><ul><li>To use on boosterpack connector and disconnect XDS-110:<br>  `J16 2:3`: Connect pin to `J1_4`</ul></ul> |
| PA20 | DEBUGSS | SWCLK | J2_13 | <ul><li>PA20 is used by SWD during debugging<br><ul><li>`J101 15:16 ON` Connect to XDS-110 SWCLK while debugging<br><li>`J101 15:16 OFF` Disconnect from XDS-110 SWCLK if using pin in application</ul></ul> |
| PA19 | DEBUGSS | SWDIO | J2_17 | <ul><li>PA19 is used by SWD during debugging<br><ul><li>`J101 13:14 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J101 13:14 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0L1306 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0L1306 User's Guide](https://www.ti.com/lit/slau869).

## Example Usage

Connect the hardware that descriped in the document. Compile, load and run the example.
Push the S2 button to start program MSPM0L1306.
