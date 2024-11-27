## Example Summary

The following example used keystore for secure storage of key and performs 256-bit AES CMAC encryption using the on-board AESADV engine with a software wrapper. 

## Peripherals & Pin Assignments
| Peripheral | Function | Pin | Launchpad Pin |
| --- | --- | --- | --- |
| GPIOA | Open-Drain Output | PA0 |
| AESADV | | | |
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

Compile, load give NRST and then run the example.

Through output file we are loading CSC enabled non-main along side main application, so give reset after loading the application.

The application will hit a SW breakpoint to allow the user to inspect the value of gCorrectResults and gSuccessfulOperation to see if the AES operation completed successfully and the results of the golden value comparison to the ciphertext after initial encryption.
