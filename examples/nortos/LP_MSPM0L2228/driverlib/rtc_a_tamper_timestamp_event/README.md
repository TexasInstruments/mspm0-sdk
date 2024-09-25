## Example Summary

Configures RTC_A to generate a time stamp event when a tamper event is detected.
TamperIO 0 configured to generate a tamper event upon a rising edge.

When a time stamp event happens, the whole RTC register from second till years is copied to the time stamp capture registers.

In case of multiple time stamp events, the time stamp capture registers are configured to hold the latest
time stamp event.

## Peripherals & Pin Assignments

- N/A

## BoosterPacks, Board Resources & Jumper Settings

- N/A


### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0L2228 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#non-sysconfig-compatible-project-migration)
for information about migrating to other MSPM0 devices.

## Example Usage

Compile, load and run the example.

Generate a rising edge on PA7 (TAMPERIO_0). This will cause a time stamp event
to occur. Upon detection of a time stamp event, USER_LED_1 will blink equal to the number
of time stamp events that have occurred so far.

The time stamp capture registers can be inspected following a tamper.
