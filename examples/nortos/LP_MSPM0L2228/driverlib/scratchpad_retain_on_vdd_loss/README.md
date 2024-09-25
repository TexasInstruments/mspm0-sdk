## Example Summary

Writes data to scratchpad memory that will be retained upon VDD loss.

Scratch pad memory is a 16- to 128-byte register based memory, which will retain data similar to a non-volatile memory as long as VBAT is supplied. This means the scratch pad memory retains data when the main VDD supply is lost or during the SHUTDOWN mode. However, the memory content won’t be retained, when the VBAT domain itself is lost.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA0 | Open-Drain Output |
| GPIOA | PA21 | Standard Output |
| SYSCTL | PA3 | LFXIN |
| SYSCTL | PA4 | LFXOUT |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L2228](https://www.ti.com/tool/LP-MSPM0L2228) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA0 | GPIOA | PA0 | J3_10 | <ul><li>PA0 is 5V tolerant open-drain so it requires pull-up<br><ul><li>`J12 1:2` Use 3.3V pull-up<br><li>`J12 2:3` Use 5V pull-up</ul><br><li>PA0 can be connected to LED3<br><ul><li>`J2 ON` Connect to LED3<br><li>`J2 OFF` Disconnect from LED3</ul></ul> |
| PA21 | GPIOA | PA21 | J3_8 | <ul><li>This pin can be used for testing purposes in boosterpack connector<ul><li>Pin can be reconfigured for general purpose as necessary</ul></ul><ul><li>PA21 can be connected to GND when used for VREF-<br><ul><li>`R4` is not soldered by default allowing the pin to be used for other functions</ul></ul> |
| PA3 | SYSCTL | LFXIN | J10_18 | <ul><li>PA3 is used for LFXT functionality by default in Launchpad<br><ul><li>`Y1` crystal is soldered and connected by default</ul><br><li>PA3 can be connected to J10_18<br><ul><li>`R43` and `J10` are not soldered soldered by default<br><li>Solder `R43` and `J10` to connect pin to J10_18</ul> |
| PA4 | SYSCTL | LFXOUT | J11_1 | <ul><li>PA4 is used for LFXT functionality by default in Launchpad<br><ul><li>`Y1` crystal is soldered and connected by default</ul><br><li>PA4 can be connected to J11_1<br><ul><li>`R44` and `J11` are not soldered soldered by default<br><li>Solder `R44` and `J11` to connect pin to J11_1</ul> |
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

Compile, load and run the example.

Run the example first with `gVDDLost` set to false. This will first write data into
scratchpad memory. Disconnect VDD from the system but leave VBAT powered.
Reconnect VDD power and re-run the example with `gVDDLost` set to true. The example
will now read back the data inside of scratchpad memory and verify that the data was retained
throughout the loss of VDD. A breakpoint will be hit and the value of `gErrorOccured` can be inspected. If all scratchpad memory was properly retained, `gErrorOccured` will be set to false. Otherwise, `gErrorOccured` will be true if any data was not retained.

USER_LED_PIN will go low to turn on a LED if the example finished running without an error.
USER_TEST_PIN will mimic the behavior of USER_LED_PIN.
