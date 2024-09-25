
## Example Summary
Two PWMs are generated for 1 second, then saves the PWM timer configuration
before going to STOP mode. The device stays in STOP mode for 1 second and then
restores PWM generation.

Some MSPM0G peripherals residing in Power Domain 1 (PD1) don't retain register contents when
entering STOP or STANDBY modes, refer to the datasheet for more details.
TimerA is a peripheral in PD1 which exhibits this behavior.
Developers can decide to re-initialize the peripheral using the default
initialization (i.e. SYSCFG_DL_TIMERA_INST_0_init) from SysConfig. This approach
is more memory-efficient.
Alternatively, this example demonstrates using DriverLib APIs to save and
restore the register configuration of the peripheral before and after entering
low-power mode. This approach is recommended if the Timer configuration is
modified at runtime.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA0 | Open-Drain Output |
| GPIOA | PA15 | Standard Output with internal pull-down |
| SYSCTL |  |  |
| TIMA1 | PB4 | Capture/Compare Pin 0 |
| TIMA1 | PB1 | Capture/Compare Pin 1 |
| TIMG7 | PA28 | Capture/Compare Pin 0 |
| TIMG7 | PA31 | Capture/Compare Pin 1 |
| TIMG0 |  |  |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA0 | GPIOA | PA0 | J27_9 | <ul><li>PA0 is 5V tolerant open-drain so it requires pull-up<br><ul><li>`J19 1:2` Use 3.3V pull-up<br><li>`J19 2:3` Use 5V pull-up</ul><br><li>PA0 can be connected to LED1<br><ul><li>`J4 ON` Connect to LED1<br><li>`J4 OFF` Disconnect from LED1</ul></ul> |
| PA15 | GPIOA | PA15 | J3_30 | <ul><li>This pin can be used for testing purposes in boosterpack connector<ul><li>Pin can be reconfigured for general purpose as necessary</ul></ul> |
| PB4 | TIMA1 | CCP0 | J4_40 | N/A |
| PB1 | TIMA1 | CCP1 | J4_39 | N/A |
| PA28 | TIMG7 | CCP0 | J4_38 | N/A |
| PA31 | TIMG7 | CCP1 | J4_37 | N/A |
| PA20 | DEBUGSS | SWCLK | N/A | <ul><li>PA20 is used by SWD during debugging<br><ul><li>`J101 15:16 ON` Connect to XDS-110 SWCLK while debugging<br><li>`J101 15:16 OFF` Disconnect from XDS-110 SWCLK if using pin in application</ul></ul> |
| PA19 | DEBUGSS | SWDIO | N/A | <ul><li>PA19 is used by SWD during debugging<br><ul><li>`J101 13:14 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J101 13:14 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0G3507 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0G3507 User's Guide](https://www.ti.com/lit/slau873).

## Example Usage
Compile, load and run the example.
Monitor PWM generation before and after STOP mode.
USER_TEST_PIN will go high when 1 sec timeout counter is started and go low
when the 1 sec period has completed. USER_LED_PIN will go low to turn on a LED
once the example finished running without an error.
