## Example Summary

The example is created to demo a MSPM0 Level 2 gauge solution based on MSPM0 internal OPAs. This solution can self-calibrate the voltage offset and GAIN setting of OPAs and do dynamic GAIN setting to reach high accuracy across low to high current.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA2 | Standard Output |
| GPIOA | PA3 | Standard Output |
| SYSCTL |  |  |
| TIMG0 |  |  |
| UART1 | PA1 | RX Pin |
| UART1 | PA0 | TX Pin |
| ADC0 | PA25 | ADC12 Channel 2 Pin |
| VREF |  |  |
| OPA1 |  |  |
| OPA0 |  |  |
| EVENT |  |  |
| DMA |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L1306](https://www.ti.com/tool/LP-MSPM0L1306) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA2 | GPIOA | PA2 | N/A | <ul><li>PA2 is used for ROSC functionality by default in Launchpad<br><ul><li>`R6` is connected to GND by default<br><li>Remove `R6` to use pin for other functions</ul></ul> |
| PA3 | GPIOA | PA3 | J2_19 | N/A |
| PA1 | UART1 | RX | J1_9 | <ul><li>PA1 is 5V tolerant open-drain so it requires pull-up<br><ul><li>`J19 2:3` Use 3.3V pull-up<br><li>`J9 1:2` Use 5V pull-up</ul></ul> |
| PA0 | UART1 | TX | J1_10 | <ul><li>PA0 is 5V tolerant open-drain so it requires pull-up<br><ul><li>`J10 2:3` Use 3.3V pull-up<br><li>`J10 1:2` Use 5V pull-up</ul><br><li>PA0 can be connected to LED1<br><ul><li>`J2 OFF` Disconnect from LED1</ul></ul> |
| PA25 | ADC0 | A2 | J1_2 | <ul><li>PA25 can be connected to photodiode D1<br><ul><li>`J4 OFF` Disconnect from photodiode D1</ul></ul> |
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

This example was developed and tested based on MSPM0L1306. Please refer to the related [app note](https://www.ti.com/lit/slaaee6) for more details.
