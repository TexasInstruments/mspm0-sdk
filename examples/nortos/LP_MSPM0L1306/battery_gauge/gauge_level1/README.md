## Example Summary

This example shows the implementation of using MSPM0 to realize a voltage gauge solution. After inputting a battery model and finishing parameter setting, users can get the runtime SOC with only the voltage input.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| UART1 | PA0 | TX for data transmission |
| UART1 | PA1 | RX for data receiving |
| ADC0 | PA21 | ADC12 Channel 5 Pin for temperature detection |
| ADC0 | PA26 | ADC12 Channel 1 Pin for battery voltage detection |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

- Please refer to the related [app note](https://www.ti.com/lit/slaaee3) for more hardware details.


### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0L1306 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#non-sysconfig-compatible-project-migration)
for information about migrating to other MSPM0 devices.

## Example Usage

This example was developed and tested based on MSPM0L1306. Please refer to the related [app note](https://www.ti.com/lit/slaaee3) for more details.
