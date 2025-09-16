## Example Summary

#### MSP-DALI Occupancy Sensor Demo
This example shows how to utilize Parts 103 and 303 from MSP DALI Library to create a simple DALI Input Device - Occupancy Sensor. It uses a PIR sensor on the TIDA 010-963 board to detect motion and send event messages over the DALI bus.


## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| TIMA0 | PB0  | Capture/Compare Pin 2 |
| TIMG6 | PB7 | Capture/Compare Pin 1 |
| TIMG0 |  |  |
| TIMG7 |  |  |
| TIMG8 |  |  |
| TIMG12 |  |  |
| GPIOA | PA22 | Standard Input |
| GPIOB | PB18 | Standard Output |
| TRNG |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

Visit [TIDA-010963](https://www.ti.com/tool/TIDA-010963) for reference design information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PB0 | TIMA0 | Capture/Compare Pin 2 | J2_12 |  |
| PB7 | TIMG6 | Capture/Compare Pin 1 | J2_14 |  |
| PB18 | GPIOB | Standard Output | J3_25 |  |
| PA22 | GPIOA | Standard Output | J3_25 | <ul><li>PA22 can be connected to photodiode D1 <ul><li>`J16 OFF` Disconnect from photodiode D1 </ul></ul> |
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
MSPM0 LaunchPad, please visit the [LP-MSPM0G3507 User's Guide](https://www.ti.com/lit/slau873d).

## Example Usage

Connect the J6 connector on the TIDA 010-963 Boosterpack to enable PIR Sensor.
Compile, load and run the example.

Please refer to MSPM0 DALI User's Guide for more details.
