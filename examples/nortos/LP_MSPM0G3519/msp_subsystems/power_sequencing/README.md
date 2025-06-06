## Example Summary

This example creates a power sequence that allows for devices/pins be turned off and on in different sequences. 

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOA | Standard Output | PA22 | J8_12 |  |
| GPIOA | Standard with Wake Output | PA18 | S1 | Populate Jumper(s): J1[1:2] |
| GPIOA | Standard with Wake Output | PA17 | J7_25 |  |
| GPIOA | Standard Output | PA15 | J3_30 |  |
| GPIOB | Standard Input with internal pull-up | PB13 | J8_19 |  |
| SYSCTL |  |  |  |  |
| TIMG0 |  |  |  |  |
| EVENT |  |  |  |  |
| BOARD | Debug Clock | PA20 | J19_16 |  |
| BOARD | Debug Data In Out | PA19 | J19_14 |  |

Visit [LP_MSPM0G3519](https://www.ti.com/tool/LP-MSPM0G3519) for LaunchPad information, including user guide and hardware files.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0G3519 web page](https://www.ti.com/tool/LP-MSPM0G3519).

## Example Usage

Compile, load, and run the example.
Connect the output GPIOs to external devices or connect a logic analyzer to see the sequence.
To start the example apply a voltage to the input pin.
During the sequence the different pins will set high after a designated amount of time after each one is turned on.
