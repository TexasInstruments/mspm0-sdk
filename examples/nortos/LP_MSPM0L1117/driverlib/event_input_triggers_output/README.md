## Example Summary
This example demonstrates how to publish and subscribe events in GPIOA
without CPU intervention.

GPIOA publishes events via USER_SWITCH_1 to GPIO_EVENT_CHANNEL and is also
subscribed to GPIO_EVENT_CHANNEL. GPIOA subscriber is configured to toggle
USER_LED_1 every time a rising or falling edge is detected on USER_SWITCH_1.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOA | Open-Drain Output | PA0 | Red LED1 | Populate Jumper(s): J1[1:2] |
| GPIOA | Standard Input with internal pull-up | PA5 | J21_1 | Populate Component(s): J21 |
 !!!MULTI!!!
| GPIOA | Standard Input with internal pull-up | PA5 | S2 | Populate Jumper(s): J6[1:2] |
 !!!MULTI!!!
| SYSCTL |  |  |  |  |
| EVENT |  |  |  |  |
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
Compile, load and run the example. LED1 toggles every time a rising or falling
edge is detected on S2.