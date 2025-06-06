## Example Summary

#### MSP-DALI LED Demo
This example shows how to utilize Parts 102 and 207 from MSP DALI Library to create a simple LED Demo.  This demo allows you to utilize the various DALI commands to control an LED on the launchpad and keep track of pertinent DALI
information.


## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| TIMG14 | PA0 | Capture/Compare Pin 0 |
| TIMG14 | PA1 | Capture/Compare Pin 1 |
| TIMG8 |  |  |
| TIMA0 | PA9 | Capture/Compare Pin 1 |
| TIMG1 |  |  |
| UART0 | PA10 | TX Pin |
| EVENT |  |  |
| CRC |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0H3216](https://www.ti.com/tool/LP-MSPM0H3216) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA0 | TIMG14 | CCP0 | J4_34 | <ul><li>`J4 ON` Connect to LED1<br><li>`J4 OFF` Disconnect from LED1</ul></ul> |
| PA1 | TIMG14 | CCP1 | | |
| PA9 | TIMA0 | CCP1 | J2_14 | N/A |
| PA10 | UART0 | TX | J2_15 | N/A |
| PA20 | DEBUGSS | SWCLK | J2_13 | <ul><li>PA20 is used by SWD during debugging<br><ul><li>`J101 15:16 ON` Connect to XDS-110 SWCLK while debugging<br><li>`J101 15:16 OFF` Disconnect from XDS-110 SWCLK if using pin in application</ul></ul> |
| PA19 | DEBUGSS | SWDIO | J2_17 | <ul><li>PA19 is used by SWD during debugging<br><ul><li>`J101 13:14 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J101 13:14 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0H3216 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.


## Example Usage

After connecting the TIDA 010-963 Boosterpack to the LP_MSPM0H3216, connect pin PA10 to J2_12 Pin on boosterpack, which maps to PA7 on the launchpad.
Compile, load and run the example.

Please refer to MSPM0 DALI User's Guide for more details.