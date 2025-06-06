## Example Summary

The following example configures the device to be an I2C target with the ADC configured with 12 bit resolution and 12.5us sampling time.
The example will process 3 commands to preform different functions. When the controller sends 0x01 the target will take an ADC sample and store it.
When the controller sends 0x02 the target will increment the Txlen for when data is requested.
When the controller sends 0x03 the target will take an ADC sample, store it, then increment Txlen.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOA | Standard Output | PA26 | J1_5 |  |
| SYSCTL |  |  |  |  |
| I2C0 | I2C Serial Data line (SDA) | PA0 | J1_10 |  |  |  |  |
| BOARD | Debug Clock | PA20 | J22_16 |  |
| BOARD | Debug Data In Out | PA19 | J22_14 |  |

Visit [LP_MSPM0L1117](https://www.ti.com/tool/LP-MSPM0L1117) for LaunchPad information, including user guide and hardware files.

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0L1117 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0L1117 User's Guide](https://www.ti.com/lit/slau953).

## Example Usage

Compile, load and run the example. Connect a valid voltage from 0-VCC to the ADC input pin. Send I2C commands from the controller to the I2C target. Valid commands are listed below.

- Command 0x01: Sample ADC
- Command 0x02: Increment the Tx Length
- Command 0x03: Sample ADC and increment Tx Length.

The i2c_controller_rw_multibyte_fifo_interrupts example can be used to assist with creating a controller to send commands to this example.
