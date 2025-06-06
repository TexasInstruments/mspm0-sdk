## Example Summary

This example creates a power sequence that allows for devices/pins be turned off and on in different sequences. 

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOA | Standard Input | PA12 | J3_25 |  |
| GPIOA | High-Drive Output | PA15 | J3_30 |  |
| GPIOA | High-Drive Output | PA16 | J3_29 | Populate Jumper(s): J15[1:2] |
| GPIOA | High-Drive Output | PA17 | J3_28 |  |
| GPIOA | High-Drive Output | PA18 | J3_26 |  |
| SYSCTL |  |  |  |  |
| TIMG0 |  |  |  |  |
| EVENT |  |  |  |  |
| BOARD | Debug Clock | PA20 | J22_16 |  |
| BOARD | Debug Data In Out | PA19 | J22_14 |  |

Visit [LP_MSPM0L1117](https://www.ti.com/tool/LP-MSPM0L1117) for LaunchPad information, including user guide and hardware files.

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0L1117 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0L1117 User's Guide](https://www.ti.com/lit/slau953).

## Example Usage

Compile, load, and run the example.
Connect the output GPIOs to external devices or connect a logic analyzer to see the sequence.
To start the example apply a voltage to the input pin.
During the sequence the different pins will set high after a designated amount of time after each one is turned on.

