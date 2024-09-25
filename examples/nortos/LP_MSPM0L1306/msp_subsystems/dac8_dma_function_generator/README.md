## Example Summary

This project shows how to use the 8 bit reference DAC included in the MSPM0 Comparator module to output an array of 8 bit samples at regular intervals using the DMA. The DAC8 must be buffered by one of the device's internal OPAs before it can be output to a pin. 

After running the code, a sinusoid will be output on PA22 with a frequency of roughly 98Hz. 

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| TIMG0 |  |  |
| COMP0 |  |  |
| OPA0 | PA22 | OPA Output Pin |
| EVENT |  |  |
| DMA |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L1306](https://www.ti.com/tool/LP-MSPM0L1306) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA22 | OPA0 | OUT | J1_8 | <ul><li>PA22 can be connected to photodiode D1<br><ul><li>`J6 OFF` Disconnect from photodiode D1<br><li>`J6 ON` Connect to photodiode D1</ul><br><li>PA22 can be connected to thermistor RT1<br><ul><li>`R95/C57` RC circuit for thermistor RT1 is not soldered by default<br><li>Solder `R95/C57` RC circuit to connect to thermistor RT1</ul></ul> |
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

Compile, load and run the example. Monitor the OPA0 output on PA22 to see the DAC8 value updating every 40us. The output will be a sinusoid from 0 V to 3.3 V, with a frequency around 98Hz. 
