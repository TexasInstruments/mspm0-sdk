## Example Summary

The following example configures TimerG4 to measures the time from the start of
the capture operation to the signal edge.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA0 | Open-Drain Output |
| GPIOA | PA21 | Standard Output |
| SYSCTL |  |  |
| TIMG4 | PB2 | Capture/Compare Pin 0 |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L2228](https://www.ti.com/tool/LP-MSPM0L2228) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA0 | GPIOA | PA0 | J3_10 | <ul><li>PA0 is 5V tolerant open-drain so it requires pull-up<br><ul><li>`J12 1:2` Use 3.3V pull-up<br><li>`J12 2:3` Use 5V pull-up</ul><br><li>PA0 can be connected to LED3<br><ul><li>`J2 ON` Connect to LED3<br><li>`J2 OFF` Disconnect from LED3</ul></ul> |
| PA21 | GPIOA | PA21 | J3_8 | <ul><li>This pin can be used for testing purposes in boosterpack connector<ul><li>Pin can be reconfigured for general purpose as necessary</ul></ul><ul><li>PA21 can be connected to GND when used for VREF-<br><ul><li>`R4` is not soldered by default allowing the pin to be used for other functions</ul></ul> |
| PB2 | TIMG4 | CCP0 | J7_39 | N/A |
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
Connect to PA10 (GPIO_TIMER_CAPTURE_C0_PIN) to an edge generator source (it can
also be connected to a momentary switch).
Compile, load and run the example.
Generate a rising edge on PA10 (GPIO_TIMER_CAPTURE_C0_PIN). This will cause the
application to hit the breakpoint instruction and the value of `edgeCapture` can
be inspected to determine the time when the edge occurred.

After the variable `edgeCapture` is inspected, the user can resume
execution and detect a new rising edge.

LED_1 will toggle every time a rising edge is detected and USER_TEST_PIN GPIO
will mimic the behavior of the LED pin on the BoosterPack header and can be used
to verify the LED behavior.
