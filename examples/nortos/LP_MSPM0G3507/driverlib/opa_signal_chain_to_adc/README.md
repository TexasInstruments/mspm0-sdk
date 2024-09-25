<span style="color:red">This example has not been fully validated at the time of this release. Functionality is not guaranteed. </span>

## Example Summary

One OPA is configured in as a Transimpedance Amplifier (TIA) with COMP0_DAC8
driving the bias voltage.
A second stage is configured as PGA with COMP1_DAC8 driving bias voltage.
The output of the second stage is connected to ADC.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| VREF | PA23 | VREF Positive (VREF+) |
| VREF | PA21 | VREF Negative (VREF-) |
| OPA0 | PA27 | OPA Inverting input 0 Pin |
| OPA0 | PA22 | OPA Output Pin |
| OPA1 | PA16 | OPA Output Pin |
| ADC1 |  |  |
| COMP0 |  |  |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA23 | VREF | VREF+ | N/A | <ul><li>PA23 can be connected to GND using C7 or to VREF- using C8<br><ul><li>`C8` is soldered by default connecting pin to VREF-<br><li>`C7` is not soldered by default and can be used for a custom capacitor to GND</ul><br><li>PA23 can be connected to boosterpack connector<br><ul><li>`R22` is not soldered by default<br><li>Solder `R22` to connect pin to J28_10</ul></ul> |
| PA21 | VREF | VREF- | J28_8 | <ul><li>PA21 can be connected to GND when used for VREF-<br><ul><li>`R3` is soldered by default connecting pin to GND</ul><br><li>PA21 can be connected to boosterpack<br><ul><li>`R10` is not soldered by default<br><li>Solder `R10` to connect pin to `J28_8`</ul></ul> |
| PA27 | OPA0 | IN0- | J1_8 | <ul><li>PA27 can be connected to photodiode D1<br><ul><li>`J17 ON` Connect to photodiode D1<br><li>`J17 OFF` Disconnect from photodiode D1</ul></ul> |
| PA22 | OPA0 | OUT | J3_24 | <ul><li>PA22 can be connected to photodiode D1<br><ul><li>`J16 ON` Connect to photodiode D1<br><li>`J16 OFF` Disconnect from photodiode D1</ul></ul> |
| PA16 | OPA1 | OUT | J3_29 | <ul><li>PA16 can be connected to boosterpack connector<br><ul><li>`J15 1:2` Connect pin to `J3_29`<br><li>`J15 2:3/OFF` Disconnect pin from `J3_29`</ul></ul> |
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

Connect OPA0_IN0- to external RC circuitry and input current to be measured.
Compile, load and run the example.  For an easier setup, keep jumpers J16 and
J17 connected to the device to utilize the light sensor circuit, which can act
as the required current source and RC circuitry for this example.  In addition,
avoid using 1x gain for the second stage programmable gain amplifier, and if no gain
is desired, configure the second stage amplifier into buffer mode.

Use debugger to check ADC result based on input current.

On powerup, the OPA and ADC pin(s) used in this example are by default set to
the correct analog mode. Therefore, calls to
DL_GPIO_initPeripheralAnalogFunction for pinmuxing those pins are not needed.
