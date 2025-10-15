## Example Summary

#### MSP-DALI LED Demo
This example shows how to utilize Parts 102 and 207 from MSP DALI Library to create a simple LED Demo.  This demo allows you to utilize the various DALI commands to control an LED on the launchpad and keep track of pertinent DALI information.


## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| TIMA0 |  |  |
| TIMG2 |  |  |
| TIMG14 | PA0 | Capture/Compare Pin 0 |
| TIMG14 | PA1 | Capture/Compare Pin 1 |
| TIMG1 | PB7 | Capture/Compare Pin 1 |
| UC4 | PB0 | UART TX Pin |
| EVENT |  |  |
| CRC |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L2117](https://www.ti.com/tool/LP-MSPM0L2117) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA0 | TIMG14 | CCP0 | J1_10 | <ul><li>PA0 is 5V tolerant open-drain so it requires pull-up<br><ul><li>`J12 2:3` Use 3.3V pull-up<br><li>`J12 1:2` Use 5V pull-up</ul><br><li>PA0 can be connected to LED1<br><ul><li>`J2 ON` Connect to LED1<br><li>`J2 OFF` Disconnect from LED1</ul></ul> |
| PA1 | TIMG14 | CCP1 | J1_9 | <ul><li>PA1 is 5V tolerant open-drain so it requires pull-up<br><ul><li>`J13 2:3` Use 3.3V pull-up<br><li>`J13 1:2` Use 5V pull-up</ul></ul> |
| PB7 | TIMG1 | CCP1 | J2_14 | N/A |
| PB0 | UC4 | TX | J4_36 | N/A |
| PA20 | DEBUGSS | SWCLK | N/A | <ul><li>PA20 is used by SWD during debugging<br><ul><li>`J14 15:16 ON` Connect to XDS-110 SWCLK while debugging<br><li>`J14 15:16 OFF` Disconnect from XDS-110 SWCLK if using pin in application</ul></ul> |
| PA19 | DEBUGSS | SWDIO | N/A | <ul><li>PA19 is used by SWD during debugging<br><ul><li>`J14 13:14 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J14 13:14 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0L2117 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

<!-- For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0L2117 User's Guide](https://www.ti.com/lit/slau928). -->

## Example Usage

After connecting the TIDA 010-963 Boosterpack to the MSPM0L2117_LP, connect pin PB0 (J4_36) to J2_12 on the launchpad.
Compile, load and run the example.

Please refer to MSPM0 DALI User's Guide for more details.
