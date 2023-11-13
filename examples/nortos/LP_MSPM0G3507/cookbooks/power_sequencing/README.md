## Example Summary

This example creates a power sequence that allows for devices/pins be turned off and on in different sequences. 

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
Connect the output GPIOs to external devices or connect a logic analyzer to see the sequence.
To start the example apply a voltage to the input pin.
During the sequence the different pins will set high after a designated amount of time after each one is turned on.
