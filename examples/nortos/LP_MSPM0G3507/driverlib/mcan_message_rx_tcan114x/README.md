## Example Summary
This example demonstrates how to receive CAN Classic and CAN-FD
messages using standard IDs using BOOSTXL-TCAN1145 boosterpack.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOB | PB22 | Standard Output |
| GPIOB | PB26 | Standard Output |
| GPIOB | PB13 | Standard Output |
| SYSCTL | PA5 | HFXIN |
| SYSCTL | PA6 | HFXOUT |
| SPI1 | PB9 | SPI SCLK (Clock) |
| SPI1 | PB8 | SPI PICO (Peripheral In, Controller Out) |
| SPI1 | PB7 | SPI POCI (Peripheral Out, Controller In) |
| SPI1 | PB6 | SPI CS0 (Chip Select 0) |
| EVENT |  |  |
| CANFD0 | PA13 | RX Pin |
| CANFD0 | PA12 | TX Pin |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PB22 | GPIOB | PB22 | J27_5 | <ul><li>PB22 can be connected to LED2 Blue<br><ul><li>`J5 ON` Connect to LED2 Blue<br><li>`J15 OFF` Disconnect from LED2 Blue</ul></ul> |
| PB26 | GPIOB | PB26 | J27_8 | <ul><li>PB26 can be connected to LED2 Red<br><ul><li>`J6 ON` Connect to LED2 Red<br><li>`J6 OFF` Disconnect from LED2 Red</ul></ul> |
| PB13 | GPIOB | PB13 | J4_35 | N/A |
| PA5 | SYSCTL | HFXIN | J28_4 | <ul><li>PA5 is used for HFXT functionality by default in Launchpad<br><ul><li>`Y2` crystal is soldered and connected by default</ul><br><li>PA5 can be connected to J28_4<br><ul><li>`R15` is not soldered soldered by default<br><li>Solder `R15` to connect pin to J28_4</ul> |
| PA6 | SYSCTL | HFXOUT | J28_5 | <ul><li>PA6 is used for HFXT functionality by default in Launchpad<br><ul><li>`Y2` crystal is soldered and connected by default</ul><br><li>PA6 can be connected to J28_5<br><ul><li>`R17` is not soldered soldered by default<br><li>Solder `R17` to connect pin to J28_5</ul> |
| PB9 | SPI1 | SCLK | J1_7 | N/A |
| PB8 | SPI1 | MOSI | J2_15 | N/A |
| PB7 | SPI1 | MISO | J2_14 | N/A |
| PB6 | SPI1 | CS0 | J2_13 | N/A |
| PA13 | CANFD0 | CANRX | J4_32/J26_3 | <ul><li>PA13 can be connected to CAN/LIN connector in addition to boosterpack connector:<br><ul><li>To use on J26 CAN/LIN connector:<br>  `R65` is populated by default and connects pin to `J26_3`</ul></ul> |
| PA12 | CANFD0 | CANTX | J4_32/J26_1 | <ul><li>PA12 can be connected to CAN/LIN connector in addition to boosterpack connector:<br><ul><li>To use on J26 CAN/LIN connector:<br>  `R64` is populated by default and connects pin to `J26_1`</ul></ul> |
| PA20 | DEBUGSS | SWCLK | N/A | <ul><li>PA20 is used by SWD during debugging<br><ul><li>`J101 15:16 ON` Connect to XDS-110 SWCLK while debugging<br><li>`J101 15:16 OFF` Disconnect from XDS-110 SWCLK if using pin in application</ul></ul> |
| PA19 | DEBUGSS | SWDIO | N/A | <ul><li>PA19 is used by SWD during debugging<br><ul><li>`J101 13:14 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J101 13:14 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0G3507 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0G3507 User's Guide](https://www.ti.com/lit/slau873).

## Example Usage
Connect the TCAN114x transceiver boosterpack as described in the TCAN
Evaluation Module User's Guide and monitor CAN-FD and CAN packages using
mcan_single_message_tx_tcan114x example project or using a CAN Bus Analyzer tool
of your choice.

The example has the following Nominal and Data Rate frequencies configured:
- Nominal Bit Rate (500 KBits/s)
- Data Bit Rate    (2 MBits/s)

Compile, load and run the example.

To turn on the LED_1, the example must receive the following package:
- ID : 0x03
- DATA[0] = 0x01

To turn off the LED_1, the example must receive the following package:
- ID : 0x03
- DATA[0] = 0x00

To turn on the LED_2, the example must receive the following package:
- ID : 0x04
- DATA[0] = 0x01

To turn off the LED_2, the example must receive the following package:
- ID : 0x04
- DATA[0] = 0x00

Note: There is a known limitation where the example might stop receiving
packages if the application halt execution after hitting a breakpoint. It is
highly recommended to use free run option while testing the example.
