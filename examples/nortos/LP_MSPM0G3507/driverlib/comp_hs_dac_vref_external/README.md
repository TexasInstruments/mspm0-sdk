## Example Summary

COMP is configured in high-speed mode using the 8-bit DAC with external
reference as the positive input, and the external pin as negative input.
An interrupt is triggered when the external input crosses the DAC treshold and
a LED is toggled accordingly.
The COMP hysteresis and filter are used to prevent multiple interrupt firing
immediately.
The device stays in SLEEP mode, except when an interrupt is fired.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA0 | Open-Drain Output |
| SYSCTL |  |  |
| VREF | PA23 | VREF Positive (VREF+) |
| COMP0 | PA27 | COMP Negative Channel 0 Pin |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA0 | GPIOA | PA0 | J27_9 | This pin is 5V tolerant open-drain and requires pull-up.<br>J4 ON/OFF: Connect/Disconnect LED1<br>J19 1:2 3.3V Pull-up<br>J19 2:3 5V Pull-up |
| PA23 | VREF | VREF+ | N/A | C8 to GND soldered by default<br>C7 to GND can be soldered for custom VREF capacitor<br>Solder R22 to connect to J28_10 |
| PA27 | COMP0 | IN0- | J1_8 | J17 OFF Disconnect from Light Sensor D1 |
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

Connect COMP0_IN0- to an external signal.
Connect external reference to VREF+.
Compile, load and run the example.
Check LED toggle when the external input crosses 1V. The example sets the DAC
threshold expecting a VREF of 3V.

On powerup, the COMP and VREF pin(s) used in this example are by default set to
the correct analog mode. Therefore, calls to
DL_GPIO_initPeripheralAnalogFunction for pinmuxing those pins are not needed.
