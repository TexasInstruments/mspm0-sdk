## Example Summary

The following example configures ADC0 and ADC1 to be simultaneously triggered
by a TimerG instance 1s after the timer has been started while the device is in
STOP0 mode. Each instance is subscribed to a split channel event.

This example also showcases how to enable HW averaging. ADC0 instance is
configured to accumulate 128 sample and then left shift the accumulated result
by 128 (Right shift results by 7). ADC1 instance is configured to accumulate 32
sample and then left shift the accumulated result by 32 (Right shift results by
5).

Once both conversions are done both results are compared and LED1 is set if
(adc0Res > adc1Res).

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA0 | Open-Drain Output |
| SYSCTL |  |  |
| TIMG0 |  |  |
| ADC0 | PA25 | ADC12 Channel 2 Pin |
| ADC1 | PA17 | ADC12 Channel 2 Pin |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA0 | GPIOA | PA0 | J27_9 | This pin is 5V tolerant open-drain and requires pull-up.<br>J4 ON/OFF: Connect/Disconnect LED1<br>J19 1:2 3.3V Pull-up<br>J19 2:3 5V Pull-up |
| PA25 | ADC0 | A2 | J1_2 | N/A |
| PA17 | ADC1 | A2 | J3_28 | R82/C51 can be soldered for RC filter |
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
Compile, load and run the example. Provide a valid voltage to ADC0_2 and ADC1_2
pin and monitor LED1 (Voltage source for ADC instances is excepted to be between
0v and MCU supply voltage).

On powerup, the ADC pin(s) used in this example are by default set to
the correct analog mode. Therefore, calls to
DL_GPIO_initPeripheralAnalogFunction for pinmuxing those pins are not needed.

## Application Design Details
N/A

## Important Note
Early experimental samples have a known issue where the user is unable to
reconnect their debugger connection after the device has gone into STOP or
STANDBY mode. Please review advisory DEBUGSS_02 to see the steps to regain
the ability to connect the device to the debugger. This information is located
in the "MSPM0G and MSPM0L Families: Functional Differences With Early Samples"
document.
