## Example Summary

This example demonstrates how to convert an analog signal to a 4kHz PWM output. The analog input signal is sampled using the MSPM0 integrated ADC. The duty cycle of the PWM output gets updated based on the ADC reading. Two timers are used in this example. One to trigger the ADC reading and another to generate the PWM output.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| TIMG4 | PA17 | Counter Compare Pin 0 |
| TIMG0 |  |  |
| ADC0 | PA27 | ADC12 Channel 0 Pin |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L1306](https://www.ti.com/tool/LP-MSPM0L1306) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA17 | TIMG4 | CCP0 | J3_25 | N/A |
| PA27 | ADC0 | A0 | J4_37 | J13 OFF: Disconnect LED2 Blue |
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

Compile, load and run the example. Apply a voltage from 0-3.3V to PA27, and monitor the PWM Duty cycle on PA17 as you change this voltage level.
