## Example Summary
LFXT (32kHz) is a source for LFCLK.
LFCLK is output to CLK_OUT pin that can be measured.

TimerG is configured in periodic mode and toggles an LED every 500 ms.
The device remains in STANDBY0 mode while waiting for timer interrupts.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOA | Open-Drain Output | PA0 | Red LED1 | Populate Jumper(s): J4[1:2] |
| SYSCTL | CLK OUT | PA27 | J1_8 |  |
| SYSCTL | LFXIN | PA3 | J1_19 | Populate Component(s): R10 |
| SYSCTL | LFXOUT | PA4 | J1_18 | Populate Component(s): R11 |
| TIMG0 |  |  |  |  |
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
MSPM0 LaunchPad, please visit the LP-MSPM0G3218 User's Guide.

## Example Usage
Compile, load and run the example. LED toggles every 500 ms.
