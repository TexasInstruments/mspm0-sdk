## Example Summary

This example demonstrates testing ECC logic by writing invalid data to inject SEC (single bit error correction) and DED (double bit error correction) errors.

First a single flash word is correctly programmed with the ECC code generated by the flash controller hardware. The ECC code that was generated for this flash word is read back from the ECC code space.

Then a SEC error is demonstrated by programming a flash word with ECC that has a one bit difference from the data that corresponds with the ECC code. A SEC error will be reported by SYSCTL, and since SEC errors are correctable, the "corrected" code will be programmed to the corrected code space and the "uncorrected" code can be read from the uncorrected code space.

Last, a DED error is demonstrated by programming a flash word with ECC that has a two bit difference from the data that corresponds with the ECC code. A DED error will be reported by SYSCTL, and a DED error will also result in an NMI (Non-Maskable Interrupt) triggering with a DED fault. DED errors are uncorrectable, so the flash word will be programmed to the corrected and uncorrected code space.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | Pin | Launchpad Pin |
| --- | --- | --- | --- |
| GPIOA | Open-Drain Output | PA0 | Red LED1 |
| GPIOA | Standard Output | PA15 | J3_30 |
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

Compile, load and run the example.
The flash operations will start automatically when the example is run.
The application will stop at different SW breakpoints so the user can verify the data and status match the expected values in the comments. The final breakpoint is placed in the NMI_Handler, since the injected DED error will trigger an NMI fault.