## Example Summary

Note: The use of "Master" and "Slave", along with "MOSI/MISO" terminology is being considered obsolete. These terms will be replaced with "Controller" and "Peripheral", and "PICO/POCI" respectively.

The following example configures the SPI as a Peripheral with the ADC configured with 12 bit resolution and 125us sample time.
The example will process 3 commands to preform different functions. When the controller sends 0x01 the peripheral will take an ADC sample and store it.
When the controller sends 0x02 the peripheral will send the current value that is stored in gADCResult.
When the controller sends 0x03 the peripheral will take an ADC sample, store it, then send the value.

The peripheral will sleep between ADC samples and SPI commands.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOA | Open-Drain Output | PA0 | Red LED1 | Populate Jumper(s): J6[1:2] |
| SYSCTL |  |  |  |  |
| SPI1 | SPI SCLK (Clock) | PB9 | J4_39 |  |
| SPI1 | SPI PICO (Peripheral In, Controller Out) | PB8 | J2_15 |  |
| SPI1 | SPI POCI (Peripheral Out, Controller In) | PB7 | J2_14 |  |
| SPI1 | SPI CS0 (Chip Select 0) | PB6 | J2_18 |  |
| ADC0 | ADC12 Channel 2 Pin | PA25 | J1_6 |  |
| EVENT |  |  |  |  |
| BOARD | Debug Clock | PA20 | J19_16 |  |
| BOARD | Debug Data In Out | PA19 | J19_14 |  |

Visit [LP_MSPM0G3519](https://www.ti.com/tool/LP-MSPM0G3519) for LaunchPad information, including user guide and hardware files.

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0G3519 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0G3519 User's Guide](https://www.ti.com/lit/slau947).

## Example Usage
Make the following connections between the SPI Controller and SPI Peripheral:
- Controller SCLK -> Peripheral SCLK
- Controller PICO -> Peripheral PICO
- Controller POCI <- Peripheral POCI
- Controller CS   -> Peripheral CS

The SPI is initialized with the following configuration:
- SPI Peripheral
- Motorola 4 Wire with Polarity 0, Phase 0
- No parity
- 16 bits per transfer
- MSB first

Compile, load and run the example. Connect a valid voltage from 0-VCC to the ADC input pin. Send SPI commands from the controller to the spi peripheral. Valid commands are listed below.

- Command 0x01: Sample ADC
- Command 0x02: Send ADC results
- Command 0x03: Sample ADC and send results.

The GPIO will toggle to indicate that a SPI command was received.


