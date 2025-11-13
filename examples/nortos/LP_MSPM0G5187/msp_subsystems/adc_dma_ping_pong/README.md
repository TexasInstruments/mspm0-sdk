## Example Summary
This example demonstrates how to create a DMA ping pong to transfer ADC data between 2 buffers. 
The ADC is using 12-bit resolution mode at 10us sampling time and using the Internal Vref at 2.5V as the ADC reference. 
The example will take 64 samples and transfer from the ADC FIFO to the first buffer via the DMA. 
The DMA starts transfer once the ADC fills all the memory registers and the DMA will transfer 6 32-bit data packets per trigger.
Once all the data is transferred the ADC module will generate the DMA done interrupt. The DMA will then be configured to the other buffer as the destination address. 

Refer to the documentation of this design at www.ti.com/lit/slaae84

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| SYSCTL |  |  |  |  |
| ADC0 | ADC12 Channel 2 Pin | PA25 | J1_2 |  |
| VREF |  |  |  |  |
| EVENT |  |  |  |  |
| DMA |  |  |  |  |
| BOARD | Debug Clock | PA20 | J101_16 |  |
| BOARD | Debug Data In Out | PA19 | J101_14 |  |
                                                                                                                                               

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0G3507 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the LP-MSPM0G5187 User's Guide.

## Example Usage
Compile, load and run the example. Input a signal within 0 - 2.5V into the ADC input. 
Inspect the buffers at the software breakpoint, the result will alternate between buffers with each continue of the program.  
