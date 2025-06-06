## Example Summary

This example can be used to correct the crystal offset error.
Button are used to increase or decrease offset while an external tool measures
frequency using RTCCLK output.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOA | Standard Input with internal pull-up | PA14 | S2 |  |
| GPIOA | Standard with Wake Input | PA18 | S2 |
| SYSCTL |  |  |  |  |
| EVENT |  |  |  |  |
| RTC_B | RTC_B Out Pin | PA0 | J4_34 |  |
| BOARD | Debug Clock | PA20 | J13_4 | Populate Component(s): J13 |
| BOARD | Debug Data In Out | PA19 | J13_2 | Populate Component(s): J13 |

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
MSPM0 LaunchPad, please visit the LP-MSPM0C1106 User's Guide.

## Example Usage
Compile, load and run the example.
The output frequency for offset calibration is by default set to 512Hz.
Connect external equipment such as oscilloscope to RTC_OUT.
Press S1 to increment offset by +1PPM, S2 to decrement correction by -1PPM.
