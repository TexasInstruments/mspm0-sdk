## Example Summary

The following example configures ADC0 to trigger single conversion by software
and turn on LED1 if ADC > 0.5*(external vref). Otherwise, LED1 remains off.

This examples configure ADC12 instance to use internal voltage reference.
## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA0 | Open-Drain Output |
| SYSCTL |  |  |
| ADC0 | PA25 | ADC12 Channel 2 Pin |
| VREF | PA23 | VREF Positive (VREF+) |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA0 | GPIOA | PA0 | J27_9 | This pin is 5V tolerant open-drain and requires pull-up.<br>J4 ON/OFF: Connect/Disconnect LED1<br>J19 1:2 3.3V Pull-up<br>J19 2:3 5V Pull-up |
| PA25 | ADC0 | A2 | J1_2 | N/A |
| PA23 | VREF | VREF+ | N/A | C8 to GND soldered by default<br>C7 to GND can be soldered for custom VREF capacitor<br>Solder R22 to connect to J28_10 |
| PA20 | DEBUGSS | SWCLK | N/A | J101 15:16 ON: Connect to XDS-110 SWCLK (debug) |
| PA19 | DEBUGSS | SWDIO | N/A | J101 13:14 ON: Connect to XDS-110 SWDIO (debug) |

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0G3507 User's Guide](https://www.ti.com/lit/slau846).

## Example Usage
Compile, load and run the example. Provide a valid voltage to ADC0_0 (Voltage
source for ADC is excepted to be between 0 - External Vref (external reference
shouldn't exceed VDD) and monitor LED1. LED1 will be turned on when ADC
conversion greater than (External Vref / 2).

On powerup, the ADC and VREF pin(s) used in this example are by default set to
the correct analog mode. Therefore, calls to
DL_GPIO_initPeripheralAnalogFunction for pinmuxing those pins are not needed.