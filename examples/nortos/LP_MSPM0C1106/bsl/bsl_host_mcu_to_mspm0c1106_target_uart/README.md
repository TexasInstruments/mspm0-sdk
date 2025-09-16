## Example Summary

This demo shows how to program a MSPM0C1106 MCU using a LP-MSPM0C1106 as host by BSL UART interface. It can support hardware invoke or software invoke.

More information please refer to [MSPM0 Bootloader (BSL) Implementation](https://www.ti.com/lit/slaae88) or [MSPM0 Bootloader user's guide](https://www.ti.com/lit/slau887)

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA14| Standard Input with internal pull-up |
| GPIOA | PA13 | Standard Output(To control NRST pin of target, just needed for hardware invoke mode) |
| GPIOA | PA12 | Standard Output(To control BSL Invoke pin of target, just needed for hardware invoke mode) |
| GPIOA | PA0 | Standard Output |
| SYSCTL |  |  |
| UART0 | PA11 | RX Pin |
| UART0 | PA10 | TX Pin |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0C1106](https://www.ti.com/tool/LP-MSPM0C1106) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA14 | GPIOA | PA14 | J2_19 | <ul><li>PA14 is connected to S2 button to GND with no external pull resistor<br><ul><li>Press `S2` button to connect pin to GND<br><li>Don't use `S2` button if not needed by application</ul></ul> |
| PA12 | GPIOA | PA12 | J2_17 | Needed only for Hardware invoke |
| PA13 | GPIOA | PA13 | J2_18 | Needed only for Hardware invoke |
| PA0 | GPIOA | PA0 | J2_34| <ul><li>PA0 can be connected to LED1<br><ul><li>`J4 ON` Connect to LED1<br><li>`J4 OFF` Disconnect from LED1</ul></ul> |
| PA11 | UART0 | RX | J2_35 | <ul><li>PA11 can be used as UART RX connected to boosterpack connector</ul></ul> |
| PA10 | UART0 | TX |J2_15| <ul><li>PA10 can be used as UART TX connected to boosterpack connector</ul></ul> |
| PA20 | DEBUGSS | SWCLK | N/A | <ul><li>PA20 is used by SWD during debugging<br><ul><li>`J101 15:16 ON` Connect to XDS-110 SWCLK while debugging<br><li>`J101 15:16 OFF` Disconnect from XDS-110 SWCLK if using pin in application</ul></ul> |
| PA19 | DEBUGSS | SWDIO | N/A | <ul><li>PA19 is used by SWD during debugging<br><ul><li>`J101 13:14 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J101 13:14 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0C1106 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0C1106 User's Guide](https://www.ti.com/lit/slau950).

## Example Usage

Connect the hardware as described in the document. Compile, load, and run the example.
Push the S2 button to start programming the MSPM0C1106.

> **Note:** To use software trigger mode, ensure that the application code (`bsl_software_invoke_app_demo`) exists on the target and the global define `Software_Invoke` is enabled in the project.
