## Example Summary

The following example configures ADC0 to sample channel 2 at 20kHz in Auto mode. The signal applied to the ADC input is put through an N=1 digital IIR filter. The defined beta value controls the IIR filter roll off over frequency. The MSPM0G internal Math Accelerator is used to perform the necessary multiplication operations. The filtered signal is output using the DAC12

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| ADC0 | PA25 | ADC12 Channel 2 Pin |
| EVENT |  |  |
| DAC12 | PA15 | DAC12 Output Pin |
| MATHACL |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA25 | ADC0 | A2 | J1_2 | N/A |
| PA15 | DAC12 | OUT | J3_30 | R55/C45 can be soldered for RC filter |
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
Compile, load and run the example. Input a signal on PA25 using a function generator with noise applied. Monitor the DAC0_OUT pin (PA15) with an oscilloscope to see the filtered result. 
