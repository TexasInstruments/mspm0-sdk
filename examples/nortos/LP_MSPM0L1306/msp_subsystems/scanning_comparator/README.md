## Example Summary

This example mimics multiple integrated comparators by scanning through three different configurations on COMP0. 
The comparator is operating in fast mode and utilizing the 8-bit DAC as a voltage reference. 
The three configurations include three input pins, three voltage references, and three output pins. The example configures the comparator in first configuration, delays by 15us to account for the propagation and settling delay, reads the comparator output, and then sets the corresponding GPIO with the result. This process is repeated for the other two configurations before looping back to the beginning. 

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA15 | Standard Output |
| GPIOA | PA16 | Standard Output |
| GPIOA | PA17 | Standard with Wake Output |
| SYSCTL |  |  |
| COMP0 | PA26 | COMP Positive Channel 0 Pin |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L1306](https://www.ti.com/tool/LP-MSPM0L1306) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA15 | GPIOA | PA15 | J3_23 | <ul><li>PA15 can be connected to thermistor RT1<br><ul><li>`J1 2:3/OFF` Disconnect thermistor RT1</ul></ul> |
| PA16 | GPIOA | PA16 | J3_24 | N/A |
| PA17 | GPIOA | PA17 | J3_25 | N/A |
| PA26 | COMP0 | IN0+ | J4_38 | <ul><li>PA26 can be connected to LED2 Red<br><ul><li>`J12 OFF` Disconnect from LED2 Red</ul></ul> |
| PA20 | DEBUGSS | SWCLK | J2_13 | <ul><li>PA20 is used by SWD during debugging<br><ul><li>`J101 15:16 ON` Connect to XDS-110 SWCLK while debugging<br><li>`J101 15:16 OFF` Disconnect from XDS-110 SWCLK if using pin in application</ul></ul> |
| PA19 | DEBUGSS | SWDIO | J2_17 | <ul><li>PA19 is used by SWD during debugging<br><ul><li>`J101 13:14 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J101 13:14 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0L1306 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0L1306 User's Guide](https://www.ti.com/lit/slau869).

## Example Usage

Input desired signals on PA26, PA27, and PA23.
Set voltage reference values for comparator with COMP_DACCODEA, COMP_DACCODEB, and COMP_DACCODEC.
Compile, load and run the example. 
Measure the comparator outputs PA15, PA16, and PA17. 
Please refer to the related [app note](https://www.ti.com/lit/SLAAEO6) for more details. 
