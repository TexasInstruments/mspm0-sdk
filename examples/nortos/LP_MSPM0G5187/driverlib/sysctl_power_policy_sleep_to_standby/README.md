## Example Summary

The following example explains how to change the power policy from SLEEP0
to STANDBY0 during runtime. There are two timers in use: TIMG0 and TIMG1.
TIMG1 is sourced from the SYSOSC, and can run in SLEEP but not STANDBY.
TIMG0 is sourced from the low-frequency LFCLK, and therefore can run in all
states but SHUTDOWN.
The application will start in SLEEP0, and then every 5s the power policy will
switch to STANDBY0. The power policies will continue to alternate every 5s.
TIMG1 will toggle LED1 every 250ms during the SLEEP policy, and not toggle when
the device is in STANDBY.
TIMG0 will toggle the LED2 constantly every 5000ms during both SLEEP0 and
STANDBY0.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOA | Open-Drain Output | PA0 | Red LED1 | Populate Jumper(s): J9[2:1] |
| GPIOB | Standard Output | PB13 | LED2 Blue | Populate Jumper(s): J11[2:1] |
| SYSCTL |  |  |  |  |
| TIMG0 |  |  |  |  |
| TIMG6 |  |  |  |  |
| EVENT |  |  |  |  |
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
MSPM0 LaunchPad, please visit the [LP-MSPM0G5187 User's Guide](https://www.ti.com/lit/slau967).

## Example Usage
Compile, load and run the example.
Monitor the behavior of LED1 and LED2.
LED1 will toggle for 5 seconds while the device is in SLEEP0, and then not
toggle for 5s when the device switches to STANDBY0.
LED0 will continue to toggle every 500ms.

**Note**: When debugging the application, the power access port (PWR-AP) will force
the device to remain active, and not enter the low power mode.
In order to see full functionality of this example, it is
recommended to terminate the debug session. Refer to the Debug section of the TRM
for more information about debugging in low power modes.
