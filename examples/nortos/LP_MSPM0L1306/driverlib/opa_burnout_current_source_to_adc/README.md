## Example Summary

OPA is configured in buffer mode using connecting positive input to ADC.
The burnout current source is then enabled to detect a possible short or open
condition.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| ADC0 | PA18 | ADC12 Channel 7 Pin |
| OPA0 | PA25 | OPA Non-inverting input 0 Pin |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L1306](https://www.ti.com/tool/LP-MSPM0L1306) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA18 | ADC0 | A7 | J3_26 | J11 OFF: Disconnect S1 button (with pull-down)<br>J101 17:18 OFF: Disconnect from XDS-110 BSL_Invoke |
| PA25 | OPA0 | IN0- | J1_2 | J4 ON/OFF: Connect/disconnect from photodiode D1<br>J1 2:3: Connect to Thermistor RT1<br>J15 ON: Connects Thermistor RT1, if needed |
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

Connect OPA0_IN0+ to external circuitry.
Compile, load and run the example.
Use debugger to check ADC result with and without burnout current source.

## Application Design Details

This example shows an implementation of OpAmp in buffer configuration with the
capability to check for open or short connection using the OPA integrated
burnout current source.

On powerup, the OPA pin(s) used in this example are by default set to
the correct analog mode. Therefore, calls to
DL_GPIO_initPeripheralAnalogFunction for pinmuxing those pins are not needed.
