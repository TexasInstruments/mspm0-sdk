## Example Summary

The following example uses a timer in combined capture mode (configured to
measure low pulse width) to implement 1-wire target interface. The demo is
provide as a reference for implementing 1-wire EEPROM with a similar command
structure to DS2433.

Overdrive support is included but untested.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA1 | Open-Drain Output |
| GPIOA | PA4 | Standard Output |
| GPIOA | PA5 | High-Speed Output |
| SYSCTL |  |  |
| TIMG4 | PA10 | Capture/Compare Pin 0 |
| TIMG1 | PA0 | Capture/Compare Pin 0 |
| TIMG0 |  |  |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L1306](https://www.ti.com/tool/LP-MSPM0L1306) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA1 | GPIOA | PA1 | J1_9 | <ul><li>PA1 is 5V tolerant open-drain so it requires pull-up<br><ul><li>`J19 2:3` Use 3.3V pull-up<br><li>`J9 1:2` Use 5V pull-up</ul></ul> |
| PA4 | GPIOA | PA4 | J2_14 | N/A |
| PA5 | GPIOA | PA5 | J2_15 | N/A |
| PA10 | TIMG4 | CCP0 | J4_36 | N/A |
| PA0 | TIMG1 | CCP0 | J1_10 | <ul><li>PA0 is 5V tolerant open-drain so it requires pull-up<br><ul><li>`J10 2:3` Use 3.3V pull-up<br><li>`J10 1:2` Use 5V pull-up</ul><br><li>PA0 can be connected to LED1<br><ul><li>`J2 ON` Connect to LED1<br><li>`J2 OFF` Disconnect from LED1</ul></ul> |
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
Connect a 1-wire controller to PA10 (GPIO_TIMER_CAPTURE_C0_PIN). The demo
will respond to standard 1-wire commands (read/seach/match/skip ROM) and
implements the following subcommands:
 - 0x0F: Write scratchpad
 - 0xAA: Read scratchpad
 - 0x55: Copy Scratchapd
 - 0xF0: Read memory
