## Example Summary

Along with an external circuit this code amplifies the difference between two voltage inputs and outputs a single ended signal while rejecting the common-mode voltage. 

## Peripherals & Pin Assignments



## BoosterPacks and Board Resources

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.


### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0G3507 web page](https://www.ti.com/tool/LP-MSPM0G3507).

## Example Usage

Compile, load, and run the example.
Build circuit described in the top of the code in the opening comments 
Connect circuit and voltage to the proper input/outputs for the OPAs.
To read the output, connect a logic analyzer or oscilloscope to the Vo labeled in the circuit diagram or read the ADC output in CCS.
