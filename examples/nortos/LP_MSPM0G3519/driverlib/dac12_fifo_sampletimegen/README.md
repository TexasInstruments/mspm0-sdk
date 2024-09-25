## Example Summary

DAC12 sends a repetitive signal using FIFO and sample timer generator.
The sample timer generator is configured to 16KSPS, so the output frequency will be 16KSPS / (Number of samples).
The sine-wave contains 32 samples, resulting in 500Hz.
The DAC is configured to use VDDA/VSSA as reference.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | Pin | Launchpad Pin |
| --- | --- | --- | --- |
| DAC12 | DAC12 Output Pin | PA15 | J3_30 |
| BOARD | Debug Clock | PA20 | J19_16 |
| BOARD | Debug Data In Out | PA19 | J19_14 |

### Device Migration Recommendations
This project was developed for a superset device included in the MSPM0 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the LP-MSPM0G3519 User's Guide.

## Example Usage
Compile, load and run the example.
Connect an oscilloscope to check output on DAC_OUT.
Default output (OUTPUT_SIGNAL = gOutputSignalSine32) will be a sine wave at 500Hz with a maximum amplitude relative to VDDA.
OUTPUT_SIGNAL can be changed to gOutputSignalTriangle4 to output a triangle signal.
