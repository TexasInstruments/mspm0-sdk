## Example Summary

Configures different power policies based on the configuration of two input
pins. This prevents the device from indefinitely staying in SHUTDOWN
mode since the serial wire debug (SWD) remains locked while the device is in
SHUTDOWN mode.

After entering SHUTDOWN mode, the device will exit when the RESET switch S1 is pressed.
Then it will toggle the LED for as many times as it has exited
SHUTDOWN mode before (i.e. LED will toggle once the first time it wakes up, two
times the second time it wakes-up and so on).

If a different power policy is selected via the configuration input pins. The
device will only remain in the selected power policy but will not wake-up via
the RESET switch S1.

The table below shows the power policy to be configured based on configuration
input configuration.

|config input 0      |config input 1      |Power Policy        |
|--------------------|--------------------|--------------------|
| High (default)     | High (default)     | SHUTDOWN           |
| High (default)     | Low (pin grounded) | STOP0              |
| Low (pin grounded) | High (default)     | STANDBY0           |
| Low (pin grounded) | Low (pin grounded) | RUN0SLEEP0         |

**Note**: Example requires Rev E3 or later of the MSPM0C1104 LaunchPad.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA22 | Standard Output |
| GPIOA | PA6 | Standard Output |
| GPIOA | PA2 | Standard Input with internal pull-up |
| GPIOA | PA23 | Standard Input with internal pull-up |
| SYSCTL |  |  |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0C1104](https://www.ti.com/tool/LP-MSPM0C1104) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA22 | GPIOA | PA22 | J1_8 | N/A |
| PA6 | GPIOA | PA6 | J1_7 | N/A |
| PA2 | GPIOA | PA2 | J2_13 | N/A |
| PA23 | GPIOA | PA23 | J2_12 | N/A |
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

Compile, load and run the example. Rising edge on RESET switch S1 will wake device from
SHUTDOWN state and the LED blinks equal to the number of times the device has been
woken up.

Note that pressing S1 while debugging the application on CCS will cause the JTAG to disconnect
and report errors. This is expected behavior, and the application will still be running.
However, it is recommend to run this program while not debugging on CCS.
