## Example Summary

The following example configures the ADC0 to trigger single conversion and send the ADC data through UART.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA0 | Open-Drain Output |
| SYSCTL |  |  |
| UART0 | PA10 | TX Pin |
| ADC0 | PA25 | ADC12 Channel 2 Pin |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA0 | GPIOA | PA0 | J27_9 | This pin is 5V tolerant open-drain and requires pull-up.<br>J4 ON/OFF: Connect/Disconnect LED1<br>J19 1:2 3.3V Pull-up<br>J19 2:3 5V Pull-up |
| PA10 | UART0 | TX | J4_34/J26_5 | J21 1:2 Connect XDS-110 BackChannel UART on J101<br>J21 2:3 Connect to J4_34<br>R62 connects J26_5 for LIN (default)<br>J101 9:10 Connect XDS-110 BackChannel UART TX |
| PA25 | ADC0 | A2 | J1_2 | N/A |
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
Compile, load and run the example.
Provide a valid voltage to the ADC input between 0 - VCC.
Scope PA10 for the UART TX signal and see the ADC code, high byte then low byte.

