## Example Summary

DMA example with a table distributed to SRAM memory via DMA, triggered by SW.

DMA channel 0 is setup as a software triggered transfer. Upon request, the DMA
distributes a series of data from a address/data table to SRAM memory. Data
transfer is confirmed by status of boolean gVerifyResult before executing a WFI.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOA | Open-Drain Output | PA0 | Red LED1 | Populate Jumper(s): J4[1:2] |
| GPIOA | Standard Output | PA15 | J3_30 |  |
| SYSCTL |  |  |  |  |
| EVENT |  |  |  |  |
| DMA |  |  |  |  |
| BOARD | Debug Clock | PA20 | J101_16 |  |
| BOARD | Debug Data In Out | PA19 | J101_14 |  |

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
MSPM0 LaunchPad, please visit the LP-MSPM0G3218 User's Guide.

## Example Usage
Compile, load and run the example. The LED should be off at the beginning of
the program execution and will turn on at the end of the example.
Inspect gVerifyResult once example hits the breakpoint. The USER_TEST_PIN GPIO
will mimic the behavior of the LED pin on the BoosterPack header and can be
used to verify the LED behavior.
