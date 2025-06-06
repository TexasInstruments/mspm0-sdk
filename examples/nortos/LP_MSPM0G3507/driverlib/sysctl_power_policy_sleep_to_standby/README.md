## Example Summary

The following example explains how to change the power policy from SLEEP0
to STANDBY0 during runtime. There are two timers in use: TIMG0 and TIMG1.
TIMG1 is sourced from the SYSOSC, and can run in SLEEP but not STANDBY.
TIMG0 is sourced from the low-frequency LFCLK, and therefore can run in all
states but SHUTDOWN.
The application will start in SLEEP0, and then every 5s the power policy will
switch to STANDBY0. The power policies will continue to alternate every 5s.
TIMG1 will toggle LED1 every 250ms during the SLEEP policy, and not toggle when
the device is in STANDBY.
TIMG0 will toggle the LED2 constantly every 5000ms during both SLEEP0 and
STANDBY0.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA0 | Open-Drain Output |
| GPIOB | PB22 | Standard Output |
| SYSCTL |  |  |
| TIMG0 |  |  |
| TIMG8 |  |  |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA0 | GPIOA | PA0 | J27_9 | <ul><li>PA0 is 5V tolerant open-drain so it requires pull-up<br><ul><li>`J19 1:2` Use 3.3V pull-up<br><li>`J19 2:3` Use 5V pull-up</ul><br><li>PA0 can be connected to LED1<br><ul><li>`J4 ON` Connect to LED1<br><li>`J4 OFF` Disconnect from LED1</ul></ul> |
| PB22 | GPIOB | PB22 | J27_5 | <ul><li>PB22 can be connected to LED2 Blue<br><ul><li>`J5 ON` Connect to LED2 Blue<br><li>`J15 OFF` Disconnect from LED2 Blue</ul></ul> |
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
Compile, load and run the example.
Monitor the behavior of LED1 and LED2.
LED1 will toggle for 5 seconds while the device is in SLEEP0, and then not
toggle for 5s when the device switches to STANDBY0.
LED0 will continue to toggle every 500ms.

**Note**: When debugging the application, the power access port (PWR-AP) will force
the device to remain active, and not enter the low power mode.
In order to see full functionality of this example, it is
recommended to terminate the debug session. Refer to the Debug section of the TRM
for more information about debugging in low power modes.
