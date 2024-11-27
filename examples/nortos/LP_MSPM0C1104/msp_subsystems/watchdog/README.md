## Example Summary

This example shows the implementation of a basic stand alone watchdog timer.  
There are two inputs, a start/enable pin as well as a WDI pin for restarting the watchdog timeout.  
There is a WDO watchdog output (WDO) signal that is open drain and used for reseting a device once the watchdog timeout has expired.  
More advanced features such as a windowed watchdog mode can easily be added.  



## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA27 | WDT_Start Input |
| GPIOA | PA24 | WDI Input |
| GPIOA | PA0 | WDT reset output |
| SYSCTL |  |  |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0C1104](https://www.ti.com/tool/LP-MSPM0C1104) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA24 | GPIOA | PA24 | J1_5 | N/A |
| PA27 | GPIOA | PA27 | J1_4 | <ul><li>PA27 can be connected to XDS-110 backchannel<br><ul><li>`J101 7:8 OFF` Disconnect XDS-110 backchannel UART TX</ul></ul> |
| PA0 | GPIOA | PA0 | J1_10 | <ul><li>PA0 is 5V tolerant open-drain so it requires pull-up<br><ul><li>`J20 1:2` Use 3.3V pull-up<br><li>`J20 2:3` Use 5V pull-up</ul></ul> |
| PA20 | DEBUGSS | SWCLK | J2_11 | <ul><li>PA20 is used by SWD during debugging<br><ul><li>`J101 13:14 ON` Connect to XDS-110 SWCLK while debugging<br><li>`J101 13:14 OFF` Disconnect from XDS-110 SWCLK if using pin in application</ul></ul> |
| PA19 | DEBUGSS | SWDIO | J2_17 | <ul><li>PA19 is used by SWD during debugging<br><ul><li>`J101 11:12 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J101 11:12 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0C1104 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0C1104 User's Guide](https://www.ti.com/lit/slau908).

## Example Usage

All inputs and outputs are active low.  
Pull WDT_Start (PA27) low to start the WDT timeout.  Toggle WDI (PA24) low within the WDT timeout period 
to reset the timeout period.  If the WDT timeout ever expires before WDI is toggled low, then the WDO signal
is pulled low to reset the monitored device.  

WDT timeout periods and length of WDO reset pulses can be configured in software in the watchdog_hal.h file.  


