## Example Summary

Configures different power policies based on the configuration of two input
pins. Which allows user to prevent the device to indefinately stay in SHUTDOWN
mode since the serial wire debug (SWD) remain locked while the device is in
SHUTDOWN mode.

After entering SHUTDOWN mode, the device will exit when a high level is detected
on the wake-up pin. Then it will toggle LED2 for as many times as it has exited
SHUTDOWN mode before (i.e. LED2 will toggle once the first time it wakes up, two
times the second time it wakes-up and so on).

If a different power policy is selected via the configuration input pnis. The
device will only remain in the selected power policy but will not wake-up via
wake-up pin

The table below shows the power policy to be configured based on configuration
input configuration.

|config input 0      |config input 1      |Power Policy        |
|--------------------|--------------------|--------------------|
| High (default)     | High (default)     | SHUTDOWN           |
| High (default)     | Low (pin grounded) | STOP0              |
| Low (pin grounded) | High (default)     | STANDBY0           |
| Low (pin grounded) | Low (pin grounded) | RUN0SLEEP0         |

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA18 | Standard with Wake Input |
| GPIOA | PA23 | Standard Output |
| GPIOA | PA0 | Open-Drain Output |
| GPIOA | PA14 | Standard Input with internal pull-up |
| GPIOA | PA28 | High-Drive Input with internal pull-up |
| SYSCTL |  |  |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L2228](https://www.ti.com/tool/LP-MSPM0L2228) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA18 | GPIOA | PA18 | J3_23 | <ul><li>PA18 can be connected to S2 button to VCC with external pull-down<br><ul><li>`J1 ON` Connect S2 button and external pull-down<br><li>`J1 OFF` Disconnect S2 button and external pull-down</ul><br><li>PA18 can be connected to XDS-110 BSL_Invoke<br><ul><li>`J14 17:18 OFF` Disconnect pin to XDS-110 BSL Invoke</ul></ul> |
| PA23 | GPIOA | PA23 | N/A | <ul><li>PA23 can be connected to VCC using R8 or to GND using C9<br><ul><li>`R8` and `C9` are not soldered by default allowing the pin to be used for other functions</ul><br><li>PA23 can be connected to LED4 Blue<br><ul><li>`J4 ON` Connect to LED4 Blue<br><li>`J4 OFF` Disconnect from LED4 Blue</ul></ul> |
| PA0 | GPIOA | PA0 | J3_10 | <ul><li>PA0 is 5V tolerant open-drain so it requires pull-up<br><ul><li>`J12 1:2` Use 3.3V pull-up<br><li>`J12 2:3` Use 5V pull-up</ul><br><li>PA0 can be connected to LED3<br><ul><li>`J2 ON` Connect to LED3<br><li>`J2 OFF` Disconnect from LED3</ul></ul> |
| PA14 | GPIOA | PA14 | J3_26 | <ul><li>PA14 has a capacitor to be used for LCDCAP1 functionality by default in Launchpad<br><ul><li>Remove `C50` if needed to use pin for other functions</ul></ul> |
| PA28 | GPIOA | PA28 | J7_34 | N/A |
| PA20 | DEBUGSS | SWCLK | N/A | <ul><li>PA20 is used by SWD during debugging<br><ul><li>`J14 15:16 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J14 15:16 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |
| PA19 | DEBUGSS | SWDIO | N/A | <ul><li>PA19 is used by SWD during debugging<br><ul><li>`J14 13:14 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J14 13:14 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0L2228 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0L2228 web page](https://www.ti.com/tool/LP-MSPM0L2228).

## Example Usage

Compile, load and run the example. Rising edge in S2 will wake device from
SHUTDOWN state and LED2 blinks equal to the number of times the device has been
woken up.
