## Example Summary
The following example calculates checksum on a block of memory using a DMA block
transfers with following configuration:
- CRC16
- Little endian
- Not reversed

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA0 | Open-Drain Output |
| GPIOA | PA1 | Open-Drain Output |
| SYSCTL |  |  |
| EVENT |  |  |
| DMA |  |  |
| CRC |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

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
MSPM0 LaunchPad, please visit the LP-MSPM0H3216 User's Guide.

## Example Usage
Compile, load and run the example. The debugger will stop at a SW breakpoint,
and the user can verify the following:
- crcCheckSumMatch is set to true
- crcChecksum is equal to CRC_EXPECTED_RESULT
- The Red LED is turned on
- The USER_TEST_PIN will mimic the behavior of the LED
