## Example Summary

The following example configures the RTC in Calendar mode and enables Alarm 1.
The LED will toggle when the alarm is triggered.
The device remains in STANDBY mode while waiting for an interrupt.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA0 | Open-Drain Output |
| GPIOA | PA15 | Standard Output |
| SYSCTL |  |  |
| EVENT |  |  |
| DMA |  |  |
| RTC |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

Visit [LP_MSPM0G3519](https://www.ti.com/tool/LP-MSPM0G3519) for LaunchPad information, including user guide and hardware files.

### Device Migration Recommendations
This project was developed for a superset device included in the LaunchPad. Please
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
The RTC format used can be set be setting either RTC_FORMAT_BINARY or
RTC_FORMAT_BCD.
If RTC_FORMAT_BINARY is set to 1, binary format will be used.
If RTC_FORMAT_BCD is set to 1, BCD format will be used.
By default, the example uses binary format and sets RTC_FORMAT_BINARY to 1.

The RTC Calendar is initialized to the date April 5th, Monday, 2021 at
10:32:58 (HH:MM:SS).
The RTC Alarm 1 is set to trigger on the 5th of the month, Monday, 10:33.
Using these default configurations, the alarm will trigger 2 seconds after the
RTC clock is started.

Compile, load and run the example.
LED1 will turn on to indicate the RTC clock has been started.
After 2 seconds, the LED will begin to toggle to indicate that the Alarm 1 was
successfully triggered. The USER_TEST pin will toggle once to indicate that the Alarm 1 was successfully triggered.
