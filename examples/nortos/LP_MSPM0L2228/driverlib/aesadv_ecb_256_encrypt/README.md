## Example Summary

The following example performs 256-bit AES ECB encryption using the on-board AESADV engine with a software wrapper.

## Peripherals & Pin Assignments

N/A

## BoosterPacks, Board Resources & Jumper Settings

N/A

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the MSPM0 LaunchPad, please visit the LP-MSPM0L2228 User's Guide (link to be determined).


### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0L2228 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#non-sysconfig-compatible-project-migration)
for information about migrating to other MSPM0 devices.

## Example Usage
Compile, load and run the example.
The application will hit a SW breakpoint to allow the user to inspect the value of gCorrectResults and gSuccessfulOperation to see if the AES operation completed successfully and the results of the golden value comparison to the ciphertext after initial encryption.
