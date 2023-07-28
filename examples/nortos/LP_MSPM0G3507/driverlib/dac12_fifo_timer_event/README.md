## Example Summary

DAC12 sends a sawtooth signal using a hardware timer trigger.
The timer trigger is configured to 32768Hz/1/32 = 1024Hz.
The output frequency of the sawtooth signal will be 1024 / (Number of samples).
Since the sawtooth counts to 4096 in increments of 8, the number of steps is 512, thus resulting in a frequency of 2Hz.
The DAC is configured to use VDDA/VSSA as reference.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| TIMG0 |  |  |
| EVENT |  |  |
| DAC12 | PA15 | DAC12 Output Pin |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA15 | DAC12 | OUT | J3_30 | R55/C45 can be soldered for RC filter |
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
Connect an oscilloscope to check output on DAC_OUT.
Default output will be a sawtooth signal at 2Hz with a maximum amplitude relative to VDDA.

On powerup, the DAC pin(s) used in this example are by default set to
the correct analog mode. Therefore, calls to
DL_GPIO_initPeripheralAnalogFunction for pinmuxing those pins are not needed.
