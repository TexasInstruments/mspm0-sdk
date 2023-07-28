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
| PA26 | GPIOA | PA26 | J4_38 | J12 ON/OFF: Connect/Disconnect LED2 Red |
| PA27 | GPIOA | PA27 | J4_37 | J13 ON/OFF: Connect/Disconnect LED2 Blue |
| PA13 | GPIOA | PA13 | J4_39 | J3 ON/OFF: Connect/Disconnect LED2 Green |
| PA15 | ADC0 | A9 | J3_23 | J1 1:2: Connect Thermistor RT1<br>J1 2:3/OFF: Disconnect Thermistor RT1<br>R17/C9 can be soldered for RC filter<br>J15 ON: Connects Thermistor RT1, if needed |
| PA25 | OPA0 | IN0- | J1_2 | J4 ON/OFF: Connect/disconnect from photodiode D1<br>J1 2:3: Connect to Thermistor RT1<br>J15 ON: Connects Thermistor RT1, if needed |
| PA24 | OPA0 | IN0- | J1_5 | J5 ON/OFF: Connect/disconnect from photodiode D1<br>R93/R94 can be soldered for thermistor RT1<br>J15 ON: Connects Thermistor RT1, if needed |
| PA22 | OPA0 | OUT | J1_8 | J6 ON/OFF: Connect/disconnect from photodiode D1<br>R95/C57 can be soldered for thermistor RT1<br>J15 ON: Connects Thermistor RT1, if needed |
| PA20 | DEBUGSS | SWCLK | J2_13 | J101 15:16 ON: Connect to XDS-110 SWCLK (debug) |
| PA19 | DEBUGSS | SWDIO | J2_17 | J101 13:14 ON: Connect to XDS-110 SWDIO (debug) |

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
