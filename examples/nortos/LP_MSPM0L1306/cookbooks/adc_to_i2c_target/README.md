## Example Summary

The following example configures the device to be an I2C target with the ADC configured with 12 bit resolution and 12.5us sampling time.
The example will process 3 commands to preform different functions. When the controller sends 0x01 the target will take an ADC sample and store it.
When the controller sends 0x02 the target will increment the Txlen for when data is requested.
When the controller sends 0x03 the target will take an ADC sample, store it, then increment Txlen.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L1306](https://www.ti.com/tool/LP-MSPM0L1306) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
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

Compile, load and run the example. Connect a valid voltage from 0-VCC to the ADC input pin. Send I2C commands from the controller to the I2C target. Valid commands are listed below.

- Command 0x01: Sample ADC
- Command 0x02: Increment the Tx Length
- Command 0x03: Sample ADC and increment Tx Length.

The i2c_controller_rw_multibyte_fifo_interrupts example can be used to assist with creating a controller to send commands to this example.
