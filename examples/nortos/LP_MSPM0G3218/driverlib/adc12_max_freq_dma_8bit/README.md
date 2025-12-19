## Example Summary
This example demonstrate how to capture 1024 samples of a single ADC
channel using 8-bit ADC resolution mode at maximum sampling rate. A DMA
transfer is triggered every time a new result is loaded to ADC conversion memory
and will continue the transfer until 1024 samples are loaded to gADCSamples
array. Once 1024 samples are captured the ADC12 module generates a DMA done
interrupt.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| SYSCTL |  |  |  |  |
| ADC1 | ADC12 Channel 0 Pin | PA15 | J3_30 |  |
| EVENT |  |  |  |  |
| DMA |  |  |  |  |
| BOARD | Debug Clock | PA20 | J101_16 |  |
| BOARD | Debug Data In Out | PA19 | J101_14 |  |

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
MSPM0 LaunchPad, please visit the LP-MSPM0G3218 User's Guide.

## Example Usage
Compile, load and run the example. Apply voltage to ADC0 pin. Once a DMA done signal is generated the example code will hit the break point instruction and gADCSamples array can be inspected. (applied voltage / 3.3) ~= (avg value in gADCSamples / 255).
