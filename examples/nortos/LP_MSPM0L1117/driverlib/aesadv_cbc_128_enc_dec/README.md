## Example Summary

The following example performs AES 128-bit CBC encryption-decryption using the on-board AES engine with a software wrapper.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOA | Open-Drain Output | PA0 | Red LED1 | Populate Jumper(s): J1[1:2] |
| SYSCTL |  |  |  |  |
| EVENT |  |  |  |  |
| DMA |  |  |  |  |
| AESADV |  |  |  |  |
| BOARD | Debug Clock | PA20 | J22_16 |  |
| BOARD | Debug Data In Out | PA19 | J22_14 |  |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0L2228 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0L2228 web page](https://www.ti.com/tool/LP-MSPM0L2228).

## Example Usage

Compile, load and run the example.
