## Example Summary

This is an example of controlling an external segment LCD display without an LCD hardware module available in MSPM0 device.
This example supports multiple GPIO Ports for SEGMENT lines.
GPIOs for all COM lines mus tbe on same PORT.
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
| GPIO | PA8 | COM0
| GPIO | PA9 | COM1
| GPIO | PA10 | COM2
| GPIO | PA11 | COM3
| GPIO | PA12 | SEG0
| GPIO | PA13 | SEG1
| GPIO | PA14 | SEG2
| GPIO | PA15 | SEG3
| GPIO | PA16 | SEG4
| GPIO | PA17 | SEG5
| GPIO | PB7 | SEG6
| GPIO | PB8 | SEG7
| GPIO | PB9 | SEG8
| GPIO | PB10 | SEG9
| GPIO | PB11 | SEG10
| GPIO | PB12 | SEG11

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0G3507 User's Guide](https://www.ti.com/lit/slau846).

## Example Usage

Compile, load and run the example.
