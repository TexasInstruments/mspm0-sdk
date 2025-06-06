## Example Summary
This example implements a simple run to completion (RTC), round-robin scheduler. The **modules&rarr;scheduler** folder contains the scheduler and task list. The DAC8Driver, and switchDriver header and source files show the minimum requirements for building tasks using this scheduling implementation, and can be discarded and replaced with tasks that provide your desired functionality. As written, pressing the S2 button will toggle LED1 on the launchpad. Simultaenously, TIMG0 is being used to trigger the COMP0 DAC8 instance to update its output value in a stair step pattern. This output is output on PA15.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOA | Open-Drain Output | PA0 | Red LED1 | Populate Jumper(s): J6[1:2] |
| GPIOB | Standard Input with internal pull-up | PB3 | S2 | Populate Jumper(s): J23[1:2] |
| SYSCTL |  |  |  |  |
| TIMG0 |  |  |  |  |
| COMP0 |  |  |  |  |
| EVENT |  |  |  |  |
| DAC12 | DAC12 Output Pin | PA15 | J3_30 |  |
| BOARD | Debug Clock | PA20 | J19_16 |  |
| BOARD | Debug Data In Out | PA19 | J19_14 |  |

Visit [LP_MSPM0G3519](https://www.ti.com/tool/LP-MSPM0G3519) for LaunchPad information, including user guide and hardware files.

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0G3519 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0G3519 User's Guide](https://www.ti.com/lit/slau947).

## Example Usage
Compile, load and run the example.
Press S2 button to see LED1 toggle.
Measure the DAC8 output on PA15 to see the value updating every 500ms. 
