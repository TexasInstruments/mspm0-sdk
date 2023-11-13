## Example Summary

This example reads all the sensors on the BP-BASSENSORSMKII using I2C then it collects and transmits data from the MSPM0 over UART.

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

Connect the BoosterPack to the MSPM0 and the temperature ribbon to the booster pack.
Compile, load, and run the example.
Connect a terminal to read UART over Serial:COM for the output.
The sensor information should output every few seconds.
