## Example Summary

GPAMP is configured in general purpose mode with rail-to-rail configuration
enabled by default.
The inputs and output are configured for easy connection to external circuitry.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| GPAMP | PA26 | GPAMP Non-inverting input Pin |
| GPAMP | PA18 | GPAMP Inverting input Pin |
| GPAMP | PA22 | GPAMP Output Pin |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA26 | GPAMP | IN+ | J1_5 | <ul><li>PA26 can be connected to photodiode D1<br><ul><li>`J18 ON` Connect to photodiode D1<br><li>`J18 OFF` Disconnect from photodiode D1</ul><br><li>PA26 can be connected to thermistor RT1<br><ul><li>`J9 2:3` Connect to thermistor RT1<br><li>`J9 1:2/OFF` Disconnect from thermistor RT1</ul></ul> |
| PA18 | GPAMP | IN- | J3_26/J3_29 | <ul><li>PA18 can be connected to S1 button to VCC with external pull-down<br><ul><li>`J8 ON` Connect S1 button and external pull-down<br><li>`J8 OFF` Disconnect S1 button and external pull-down</ul><br><li>PA18 can be connected to XDS-110 BSL_Invoke<br><ul><li>`J101 17:18 OFF` Disconnect pin to XDS-110 BSL Invoke</ul><br><li>PA18 invokes BSL by default when high during reset<br><ul><li>Use `S1` button or XDS-110 to invoke BSL during reset </ul><br><li>PA18 can be disconnected from J8 button and J101<br><ul><li>`R23` is soldered and connects pin to J8 and J101 by default</ul><br><li>PA18 can be connected to boosterpack pin J3_29<br><ul><li>`J15 2:3` Connect pin to J3_29<br><li>`J15 1:2/OFF` Disconnect pin from J3_29</ul><br><li>PA18 can be connected to boosterpack pin J3_26<br><ul><li>`R58` is soldered and connects pin to J3_26 by default</ul><br><li>PA18 can be connected to thermistor RT1<br><ul><li>`R66/R67` are not soldered by default<br><li>Solder `R66/R67` to connect pin to thermistor RT1</ul></ul> |
| PA22 | GPAMP | OUT | J3_24 | <ul><li>PA22 can be connected to photodiode D1<br><ul><li>`J16 ON` Connect to photodiode D1<br><li>`J16 OFF` Disconnect from photodiode D1</ul></ul> |
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

Connect GPAMP_IN+ and GPAMP_IN- to external circuitry.
Compile, load and run the example.
GPAMP_OUT will be the output of the Op-Amp.

On powerup, the GPAMP pin(s) used in this example are by default set to
the correct analog mode. Therefore, calls to
DL_GPIO_initPeripheralAnalogFunction for pinmuxing those pins are not needed.

## Application Design Details

This example enables users to implement a custom analog signal chain by
connecting external circuitry to the device.
Developers can use SysConfig to modify the GPAmp default configuration.
