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
| GPIOA | PA27 | Standard Output |
| SYSCTL |  |  |
| TIMG0 |  |  |
| TIMG1 |  |  |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L1306](https://www.ti.com/tool/LP-MSPM0L1306) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA0 | GPIOA | PA0 | J1_10 | This pin is 5V tolerant open-drain and requires pull-up.<br>J2 ON/OFF: Connect/Disconnect LED1<br>J10 1:2 5V Pull-up<br>J10 2:3 3.3V Pull-up |
| PA27 | GPIOA | PA27 | J4_37 | J13 ON/OFF: Connect/Disconnect LED2 Blue |
| PA20 | DEBUGSS | SWCLK | J2_13 | J101 15:16 ON: Connect to XDS-110 SWCLK (debug) |
| PA19 | DEBUGSS | SWDIO | J2_17 | J101 13:14 ON: Connect to XDS-110 SWDIO (debug) |

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0L1306 User's Guide](https://www.ti.com/lit/slau869).

## Example Usage
Compile, load and run the example.
Monitor the behavior of LED1 and LED2.
LED1 will toggle for 5 seconds while the device is in SLEEP0, and then not
toggle for 5s when the device switches to STANDBY0.
LED0 will continue to toggle every 500ms.

## Important Note
Early experimental samples have a known issue where the user is unable to
reconnect their debugger connection after the device has gone into STOP or
STANDBY mode. Please review advisory DEBUGSS_02 to see the steps to regain
the ability to connect the device to the debugger. This information is located
in the "MSPM0G and MSPM0L Families: Functional Differences With Early Samples"
document.