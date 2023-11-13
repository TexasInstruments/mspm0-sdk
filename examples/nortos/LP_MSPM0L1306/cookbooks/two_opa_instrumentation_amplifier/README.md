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
| PA25 | OPA0 | IN0- | J1_2 | J4 ON/OFF: Connect/disconnect from photodiode D1<br>J1 2:3: Connect to Thermistor RT1<br>J15 ON: Connects Thermistor RT1, if needed |
| PA24 | OPA0 | IN0- | J1_5 | J5 ON/OFF: Connect/disconnect from photodiode D1<br>R93/R94 can be soldered for thermistor RT1<br>J15 ON: Connects Thermistor RT1, if needed |
| PA22 | OPA0 | OUT | J1_8 | J6 ON/OFF: Connect/disconnect from photodiode D1<br>R95/C57 can be soldered for thermistor RT1<br>J15 ON: Connects Thermistor RT1, if needed |
| PA18 | OPA1 | IN0+ | J3_26 | J11 OFF: Disconnect S1 button (with pull-down)<br>J101 17:18 OFF: Disconnect from XDS-110 BSL_Invoke |
| PA17 | OPA1 | IN0- | J3_25 | N/A |
| PA16 | OPA1 | OUT | J3_24 | RC Filter R19/C10 not soldered by default |
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

Compile, load, and run the example.
Build circuit described in the top of the code in the opening comments 
Connect circuit and voltage to the proper input/outputs for the OPAs.
To read the output, connect a logic analyzer or oscilloscope to the Vo labeled in the circuit diagram or read the ADC output in CCS.
