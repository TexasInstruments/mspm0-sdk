## Example Summary

This example shows the implementation of using MSPM0 to control BQ78706 for reference design TIDA-010279. The example uses BQ78706 which is a battery monitor, but the same framework can be adapted for other similar UART-based battery management ICs from TI.

## Peripherals & Pin Assignments


| Peripheral | Pin  | Function          |
| ---------- | ---- | ----------------- |
| SYSCTL     |      |                   |
| UART1      | PA8  | UART Tx           |
| UART1      | PA9  | UART Rx           |
| EVENT      |      |                   |
| DEBUGSS    | PA20 | Debug Clock       |
| DEBUGSS    | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [TIDA-010279](https://www.ti.com/tool/TIDA-010279) for reference design information, including user guide and hardware files.


| Pin  | Peripheral | Function | TIDA Pin | Reference Design Settings                                                                                                                           |
| ---- | ---------- | -------- | -------- | --------------------------------------------------------------------------------------------------------------------------------------------------- |
| PA8  | UART1      | Tx       | \        | R253                                                                                                                                                |
| PA9  | UART1      | Rx       | \        | R251                                                                                                                                                |
| PA10 | UART0      | Tx       | J16_2    | Debug UART                                                                                                                                          |
| PA11 | UART0      | Rx       | J16_1    | Debug UART                                                                                                                                          |
| PA20 | DEBUGSS    | SWCLK    | J11-4    | [Cortex Debug (10-pin) connector](https://developer.arm.com/documentation/101416/0100/Hardware-Description/Target-Interfaces/Cortex-Debug--10-pin-) |
| PA19 | DEBUGSS    | SWDIO    | J11-2    | [Cortex Debug (10-pin) connector](https://developer.arm.com/documentation/101416/0100/Hardware-Description/Target-Interfaces/Cortex-Debug--10-pin-) |

### Device Migration Recommendations

This project was developed for a MSPM0G3519. Please visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration) for information about migrating to other MSPM0 devices.

### Low-Power Recommendations

TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

## Example Usage

This example was developed and tested using a [TIDA-010279](https://www.ti.com/tool/TIDA-010279). Please refer to [TIDA-010279 Design Guide](https://www.ti.com/lit/pdf/tidufb6) for more details.
