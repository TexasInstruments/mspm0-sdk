## Example Summary

The level2 gauge solution will full utilize the ADC and OPAs on MSPM0 for high precision voltage and current detection. It will calculate the battery SOC (State of charge) with voltage and current input. This solution will output the SOH (State of health), residual SOC and related capacities. This solution will suitable for applications with high accurate SOC requirement. The full solution will include PCB, software and GUI.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| UART1 | PA0 | TX for data transmission |
| UART1 | PA1 | RX for data receiving |
| ADC0 | PA21 | ADC12 Channel 5 Pin for temperature detection |
| ADC0 | PA26 | ADC12 Channel 1 Pin for battery voltage detection |
| ADC0 | PA18 | ADC12 Channel 7 Pin |
| OPA0 | PA22 | OPA0 output as the bias voltage |
| OPA1 | PA17 | OPA1 IN0- input |
| OPA1 | PA18 | OPA1 IN+ input |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

- Please refer to the related [app note](https://www.ti.com/lit/slaaef5) for more hardware details.

## Example Usage

This example was developed and tested based on MSPM0L1306. Please refer to the related [app note](https://www.ti.com/lit/slaaef5) for more details.
