## Example Summary

This example mimics multiple integrated comparators by scanning through three different configurations on COMP0. 
The comparator is operating in fast mode and utilizing the 8-bit DAC as a voltage reference. 
The three configurations include three input pins, three voltage references, and three output pins. The example configures the comparator in first configuration, delays by 15us to account for the propagation and settling delay, reads the comparator output, and then sets the corresponding GPIO with the result. This process is repeated for the other two configurations before looping back to the beginning. 

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOA | Standard Output | PA15 | J3_27 |  |
| GPIOA | Standard Output | PA16 | J3_26 |  |
| GPIOA | High-Speed Output | PA12 | J3_28 |  |
| SYSCTL |  |  |  |  |
| COMP0 | COMP Positive Channel 0 Pin | PB21 | J1_5 |  |
| EVENT |  |  |  |  |
| BOARD | Debug Clock | PA20 | J101_16 |  |
| BOARD | Debug Data In Out | PA19 | J101_14 |  |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0L1306 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.


## Example Usage

Input desired signals on PB21, PB22, and PA17 (These are the three positive terminal channel inputs).
Set voltage reference values for comparator with COMP_DACCODEA, COMP_DACCODEB, and COMP_DACCODEC.
Compile, load and run the example. 
Measure the comparator outputs PA15, PA16, and PA17. 
Please refer to the related [app note](https://www.ti.com/lit/SLAAEO6) for more details. 
