## Example Summary

COMP is configured using the 8-bit DAC as positive input, and the external pin
as negative input. The DAC is set to 1V using VDDA as reference.
The output of the comparator triggers a timer event. The timer is configured in
Edge Time Capture mode with a capture window of 2 sec.
At the end of the window, a breakpoint is triggered to check the number of
detected captures.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | Pin | Launchpad Pin |
| --- | --- | --- | --- |
| VREF | VREF Positive (VREF+) | PA23 | J6_34 |
| COMP0 | COMP Negative Channel 0 Pin | PA27 | J3_23 |
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
Connect COMP_IN- to an external signal that varies above and below
the voltage specified by COMP_INST_DAC8_OUTPUT_VOLTAGE_mV.

Compile, load and run the example.
A breakpoint will be triggered after 1sec. Check gCaptureCounter to see the
number of rising or falling edges seen on COMP_IN-.
