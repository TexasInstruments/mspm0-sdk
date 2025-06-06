## Example Summary

The following example explains how to change the power policy from SLEEP0
to STANDBY0 during runtime. There are two timers in use: TIMG0 and TIMG1.
TIMG0 is sourced from the BUSCLK, and can run in SLEEP at 24 MHz and in STANDBY
at 32 kHz.
TIMG1 is sourced from the low-frequency LFCLK, and therefore can run in all
states but SHUTDOWN.

The application will start in SLEEP0, and then every 5s the power policy will
switch to STANDBY0. The power policies will continue to alternate every 5s.
TIMG1 will toggle LED1 every 250ms during the SLEEP policy, and not toggle when
the device is in STANDBY.

**Note**: Example requires Rev E3 or later of the MSPM0C1104 LaunchPad. When debugging
the application, the power access port (PWR-AP) will force the device to remain active.
In order to see full functionality of this example, it is recommended to terminate the debug session.
Refer to the Debug section of the TRM for more information about debugging in low power modes.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA22 | Standard Output |
| GPIOA | PA2 | Standard Output |
| SYSCTL | PA18 | CLK OUT |
| TIMG14 |  |  |
| TIMG8 |  |  |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0C1104](https://www.ti.com/tool/LP-MSPM0C1104) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA22 | GPIOA | PA22 | J1_8 | N/A |
| PA2 | GPIOA | PA2 | J2_13 | N/A |
| PA18 | SYSCTL | CLK_OUT | J2_15 | N/A |
| PA20 | DEBUGSS | SWCLK | J2_11 | <ul><li>PA20 is used by SWD during debugging<br><ul><li>`J101 13:14 ON` Connect to XDS-110 SWCLK while debugging<br><li>`J101 13:14 OFF` Disconnect from XDS-110 SWCLK if using pin in application</ul></ul> |
| PA19 | DEBUGSS | SWDIO | J2_17 | <ul><li>PA19 is used by SWD during debugging<br><ul><li>`J101 11:12 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J101 11:12 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0C1104 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0C1104 User's Guide](https://www.ti.com/lit/slau908).

## Example Usage
Compile, load and run the example.
Monitor the behavior of LED1.
LED1 will toggle for 5 seconds while the device is in SLEEP0, and then not
toggle for 5s when the device switches to STANDBY0.
