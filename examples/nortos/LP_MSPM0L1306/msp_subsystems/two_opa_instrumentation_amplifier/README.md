## Example Summary

Along with an external circuit this code amplifies the difference between two voltage inputs and outputs a single ended signal while rejecting the common-mode voltage. 

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| TIMG0 |  |  |
| ADC0 |  |  |
| OPA0 | PA25 | OPA Non-inverting input 0 Pin |
| OPA0 | PA24 | OPA Inverting input 0 Pin |
| OPA0 | PA22 | OPA Output Pin |
| OPA1 | PA18 | OPA Non-inverting input 0 Pin |
| OPA1 | PA17 | OPA Inverting input 0 Pin |
| OPA1 | PA16 | OPA Output Pin |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L1306](https://www.ti.com/tool/LP-MSPM0L1306) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA25 | OPA0 | IN0- | J1_2 | <ul><li>PA25 can be connected to photodiode D1<br><ul><li>`J4 OFF` Disconnect from photodiode D1<br><li>`J4 ON` Connect to photodiode D1</ul><br><li>PA25 can be connected to thermistor RT1<br><ul><li>`J1 1:2/OFF` Disconnect from thermistor RT1<br><li>`J1 2:3` Connect to thermistor RT1</ul></ul> |
| PA24 | OPA0 | IN0- | J1_5 | <ul><li>PA24 can be connected to photodiode D1<br><ul><li>`J5 OFF` Disconnect from photodiode D1<br><li>`J5 ON` Cconnect to photodiode D1</ul><br><li>PA24 can be connected to thermistor RT1<br><ul><li>`R93/R94` circuit for thermistor RT1 is not soldered by default<br><li>Solder `R93/R94` RC circuit to connect to thermistor RT1</ul></ul> |
| PA22 | OPA0 | OUT | J1_8 | <ul><li>PA22 can be connected to photodiode D1<br><ul><li>`J6 OFF` Disconnect from photodiode D1<br><li>`J6 ON` Connect to photodiode D1</ul><br><li>PA22 can be connected to thermistor RT1<br><ul><li>`R95/C57` RC circuit for thermistor RT1 is not soldered by default<br><li>Solder `R95/C57` RC circuit to connect to thermistor RT1</ul></ul> |
| PA18 | OPA1 | IN0+ | J3_26 | <ul><li>PA18 can be connected to S1 button to VCC with external pull-down<br><ul><li>`J11 ON` Connect S1 button and external pull-down<br><li>`J11 OFF` Disconnect S1 button and external pull-down</ul><br><li>PA18 can be connected to XDS-110 BSL_Invoke<br><ul><li>`J101 17:18 OFF` Disconnect pin to XDS-110 BSL Invoke</ul></ul> |
| PA17 | OPA1 | IN0- | J3_25 | N/A |
| PA16 | OPA1 | OUT | J3_24 | N/A |
| PA20 | DEBUGSS | SWCLK | J2_13 | <ul><li>PA20 is used by SWD during debugging<br><ul><li>`J101 15:16 ON` Connect to XDS-110 SWCLK while debugging<br><li>`J101 15:16 OFF` Disconnect from XDS-110 SWCLK if using pin in application</ul></ul> |
| PA19 | DEBUGSS | SWDIO | J2_17 | <ul><li>PA19 is used by SWD during debugging<br><ul><li>`J101 13:14 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J101 13:14 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0L1306 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0L1306 User's Guide](https://www.ti.com/lit/slau869).

## Example Usage

Compile, load, and run the example.
Build circuit described in the top of the code in the opening comments 
Connect circuit and voltage to the proper input/outputs for the OPAs.
To read the output, connect a logic analyzer or oscilloscope to the Vo labeled in the circuit diagram or read the ADC output in CCS.
