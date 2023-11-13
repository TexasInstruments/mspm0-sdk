## Example Summary

This is an example of controlling an external segment LCD display via a software method in the absence of a dedicated segment LCD hardware peripheral in an MSPM0 device.
This example supports only a single GPIO Port for segment lines.
Please see accompanying application note for full explanation of example and how to use.
Software Defined Glass LCD Solution Based on MSPM0 MCUs
www.ti.com/lit/slaaee9


## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL | --- | --- |
| TIMG0 |--- | LCD Update Clock |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |
| GPIO | PA21 | COM0
| GPIO | PA22 | COM1
| GPIO | PA23 | COM2
| GPIO | PA24 | COM3
| GPIO | PA4 | SEG0
| GPIO | PA5 | SEG1
| GPIO | PA6 | SEG2
| GPIO | PA7 | SEG3
| GPIO | PA8 | SEG4
| GPIO | PA9 | SEG5
| GPIO | PA10 | SEG6
| GPIO | PA11 | SEG7
| GPIO | PA12 | SEG8
| GPIO | PA13 | SEG9
| GPIO | PA14 | SEG10
| GPIO | PA15 | SEG11
| GPIO | PA16 | SEG12
| GPIO | PA17 | SEG13


### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the associated user guide for your chosen Launchpad.

## Example Usage

Compile, load, and run the example.
