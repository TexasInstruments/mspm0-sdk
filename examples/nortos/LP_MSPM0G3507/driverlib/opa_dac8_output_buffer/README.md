## Example Summary

OPA is configured in output buffer configuration for the 8-bit DAC.The 8-bit
DAC uses VDDA as its reference voltage.
The output is configured for easy connection to external circuitry.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| VREF | PA23 | VREF Positive (VREF+) |
| VREF | PA21 | VREF Negative (VREF-) |
| COMP0 |  |  |
| OPA0 | PA22 | OPA Output Pin |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA23 | VREF | VREF+ | N/A | <ul><li>PA23 can be connected to GND using C7 or to VREF- using C8<br><ul><li>`C8` is soldered by default connecting pin to VREF-<br><li>`C7` is not soldered by default and can be used for a custom capacitor to GND</ul><br><li>PA23 can be connected to boosterpack connector<br><ul><li>`R22` is not soldered by default<br><li>Solder `R22` to connect pin to J28_10</ul></ul> |
| PA21 | VREF | VREF- | J28_8 | <ul><li>PA21 can be connected to GND when used for VREF-<br><ul><li>`R3` is soldered by default connecting pin to GND</ul><br><li>PA21 can be connected to boosterpack<br><ul><li>`R10` is not soldered by default<br><li>Solder `R10` to connect pin to `J28_8`</ul></ul> |
| PA22 | OPA0 | OUT | J3_24 | <ul><li>PA22 can be connected to photodiode D1<br><ul><li>`J16 ON` Connect to photodiode D1<br><li>`J16 OFF` Disconnect from photodiode D1</ul></ul> |
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
OPA0_OUT will output DAC8 value.

On powerup, the OPA pin(s) used in this example are by default set to
the correct analog mode. Therefore, calls to
DL_GPIO_initPeripheralAnalogFunction for pinmuxing those pins are not needed.
