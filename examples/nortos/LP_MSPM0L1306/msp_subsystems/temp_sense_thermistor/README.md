## Example Summary

This example utilizes OPA0 and ADC0 to properly condition and read from the on board thermistor of the Launchpad associated with the target device. 

This example does not implement the proper voltage to temperature conversion, as those equations and method vary by thermistor manufacturer. 
Comments are placed in code for where to add such calculations to get exact temperature readings. 
To show case actual ADC readings, this example takes an initial reading of the thermistor, 
then changes LED colors depending on if temperature has drifted above or below the initial reading.
LED Color Code:
GREEN -- Temperature has not changed
RED   -- Temperature has increased
BLUE  -- Temperature has decreased

Refer to the documentation of this design at www.ti.com/lit/slaae85 


## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA26 | Standard Output |
| GPIOA | PA27 | Standard Output |
| GPIOA | PA13 | Standard Output |
| SYSCTL |  |  |
| TIMG0 |  |  |
| ADC0 | PA15 | ADC12 Channel 9 Pin |
| OPA0 | PA25 | OPA Non-inverting input 0 Pin |
| OPA0 | PA24 | OPA Inverting input 0 Pin |
| OPA0 | PA22 | OPA Output Pin |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L1306](https://www.ti.com/tool/LP-MSPM0L1306) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA26 | GPIOA | PA26 | J4_38 | <ul><li>PA26 can be connected to LED2 Red<br><ul><li>`J12 ON` Connect to LED2 Red<br><li>`J12 OFF` Disconnect from LED2 Red</ul></ul> |
| PA27 | GPIOA | PA27 | J4_37 | <ul><li>PA27 can be connected to LED2 Blue<br><ul><li>`J13 ON` Connect to LED2 Blue<br><li>`J13 OFF` Disconnect from LED2 Blue</ul></ul> |
| PA13 | GPIOA | PA13 | J4_39 | <ul><li>PA13 can be connected to LED2 Green<br><ul><li>`J3 ON` Connect to LED2 Green<br><li>`J3 OFF` Disconnect from LED2 Green</ul></ul> |
| PA15 | ADC0 | A9 | J3_23 | <ul><li>PA15 can be connected to thermistor RT1<br><ul><li>`J1 2:3/OFF` Disconnect thermistor RT1<br><li>`J1 1:2` Connect thermistor RT1</ul><br><li>When using thermistor RT1:<br><ul><li>`J15 OFF` Disconnect power from thermistor RT1<br><li>`J15 ON` Connect power to thermistor RT1</ul></ul> |
| PA25 | OPA0 | IN0- | J1_2 | <ul><li>PA25 can be connected to photodiode D1<br><ul><li>`J4 OFF` Disconnect from photodiode D1<br><li>`J4 ON` Connect to photodiode D1</ul><br><li>PA25 can be connected to thermistor RT1<br><ul><li>`J1 1:2/OFF` Disconnect from thermistor RT1<br><li>`J1 2:3` Connect to thermistor RT1</ul></ul> |
| PA24 | OPA0 | IN0- | J1_5 | <ul><li>PA24 can be connected to photodiode D1<br><ul><li>`J5 OFF` Disconnect from photodiode D1<br><li>`J5 ON` Cconnect to photodiode D1</ul><br><li>PA24 can be connected to thermistor RT1<br><ul><li>`R93/R94` circuit for thermistor RT1 is not soldered by default<br><li>Solder `R93/R94` RC circuit to connect to thermistor RT1</ul></ul> |
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

Compile, load and run the example.
Cool or heat portion of LaunchPad where thermistor is located to see LEDs change color. 

On powerup, the OPA pin(s) used in this example are by default set to
the correct analog mode. Therefore, calls to
DL_GPIO_initPeripheralAnalogFunction for pinmuxing those pins are not needed.
