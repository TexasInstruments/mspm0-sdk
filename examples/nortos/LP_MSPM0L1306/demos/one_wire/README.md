## Example Summary

The following example uses a timer in combined capture mode (configured to
measure low pulse width) to implement 1-wire target interface. The demo is
provide as a reference for implementing 1-wire EEPROM with a similar command
structure to DS2433.

Overdrive support is included but untested.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA1 | Open-Drain Output |
| GPIOA | PA4 | Standard Output |
| GPIOA | PA5 | High-Speed Output |
| SYSCTL |  |  |
| TIMG4 | PA10 | Counter Compare Pin 0 |
| TIMG1 | PA0 | Counter Compare Pin 0 |
| TIMG0 |  |  |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L1306](https://www.ti.com/tool/LP-MSPM0L1306) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA1 | GPIOA | PA1 | J1_9 | This pin is 5V tolerant open-drain and requires pull-up.<br>J9 1:2 5V Pull-up<br>J9 2:3 3.3V Pull-up |
| PA4 | GPIOA | PA4 | J2_14 | N/A |
| PA5 | GPIOA | PA5 | J2_15 | N/A |
| PA10 | TIMG4 | CCP0 | J4_36 | N/A |
| PA0 | TIMG1 | CCP0 | J1_10 | This pin is 5V tolerant open-drain and requires pull-up.<br>J2 ON/OFF: Connect/Disconnect LED1<br>J10 1:2 5V Pull-up<br>J10 2:3 3.3V Pull-up |
| PA20 | DEBUGSS | SWCLK | J2_13 | J101 15:16 ON: Connect to XDS-110 SWCLK (debug) |
| PA19 | DEBUGSS | SWDIO | J2_17 | J101 13:14 ON: Connect to XDS-110 SWDIO (debug) |

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0L1306 User's Guide](https://www.ti.com/lit/slau869).

## Example Usage
Connect a 1-wire controller to PA10 (GPIO_TIMER_CAPTURE_C0_PIN). The demo
will respond to standard 1-wire commands (read/seach/match/skip ROM) and
implements the following subcommands:
 - 0x0F: Write scratchpad
 - 0xAA: Read scratchpad
 - 0x55: Copy Scratchapd
 - 0xF0: Read memory
