## Example Summary
This example implements a simple run to completion (RTC), round-robin scheduler. The **modules&rarr;scheduler** folder contains the scheduler and task list. The DAC8Driver, and switchDriver header and source files show the minimum requirements for building tasks using this scheduling implementation, and can be discarded and replaced with tasks that provide your desired functionality. As written, pressing the S2 button will toggle LED1 on the launchpad. Simultaenously, TIMG0 is being used to trigger the COMP0 DAC8 instance to update its output value in a stair step pattern. This output is buffered and output to PA22 via OPA0. 

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA0 | Open-Drain Output |
| GPIOB | PB21 | Standard Input with internal pull-up |
| SYSCTL |  |  |
| TIMG0 |  |  |
| COMP0 |  |  |
| VREF | PA23 | VREF Positive (VREF+) |
| OPA0 | PA22 | OPA Output Pin |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA0 | GPIOA | PA0 | J27_9 | This pin is 5V tolerant open-drain and requires pull-up.<br>J4 ON/OFF: Connect/Disconnect LED1<br>J19 1:2 3.3V Pull-up<br>J19 2:3 5V Pull-up |
| PB21 | GPIOB | PB21 | J27_4 | Connected to S2 button (no pull resistor) |
| PA23 | VREF | VREF+ | N/A | C8 to GND soldered by default<br>C7 to GND can be soldered for custom VREF capacitor<br>Solder R22 to connect to J28_10 |
| PA22 | OPA0 | OUT | J3_24 | J16 ON/OFF: Connect/Disconnect from Light sensor D1<br>RC Filter R83/C52 not soldered by default |
| PA20 | DEBUGSS | SWCLK | N/A | J101 15:16 ON: Connect to XDS-110 SWCLK (debug) |
| PA19 | DEBUGSS | SWDIO | N/A | J101 13:14 ON: Connect to XDS-110 SWDIO (debug) |

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0G3507 web page](https://www.ti.com/tool/LP-MSPM0G3507).

## Example Usage
Compile, load and run the example.
Press S2 button to see LED1 toggle.
Measure the OPA0 output on PA22 to see the DAC8 value updating every 500ms. 
