## Example Summary

Note: The use of "Master" and "Slave", along with "MOSI/MISO" terminology is
being considered obsolete. These terms will be replaced with "Controller" and
"Peripheral", and "PICO/POCI" respectively.

The following example configures the SPI as a Peripheral.
This example can be used with the spi_controller_echo_interrupts example running
 on another device.

The user must set the mode on the SPI Peripheral to the same mode
as the SPI Controller to ensure correct data communication between devices.
The example will set the SPI to Motorola 4 Wire Mode by default.

When the SPI Peripheral receives data, it will echo this data back to the SPI
Controller.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA4 | Standard Output |
| SYSCTL |  |  |
| SPI0 | PA6 | SPI SCLK (Clock) |
| SPI0 | PA18 | SPI PICO (Peripheral In, Controller Out) |
| SPI0 | PA22 | SPI POCI (Peripheral Out, Controller In) |
| SPI0 | PA17 | SPI CS1 (Chip Select 1) |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0C1104](https://www.ti.com/tool/LP-MSPM0C1104) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA4 | GPIOA | PA4 | J2_14 | N/A |
| PA6 | SPI0 | SCLK | J1_7 | N/A |
| PA18 | SPI0 | MOSI | J2_15 | N/A |
| PA22 | SPI0 | MISO | J1_8 | N/A |
| PA17 | SPI0 | CS1 | J2_18 | N/A |
| PA20 | DEBUGSS | SWCLK | J2_11 | <ul><li>PA20 is used by SWD during debugging<br><ul><li>`J101 13:14 ON` Connect to XDS-110 SWCLK while debugging<br><li>`J101 13:14 OFF` Disconnect from XDS-110 SWCLK if using pin in application</ul></ul> |
| PA19 | DEBUGSS | SWDIO | J2_17 | <ul><li>PA19 is used by SWD during debugging<br><ul><li>`J101 11:12 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J101 11:12 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0C1104 web page](https://www.ti.com/tool/LP-MSPM0C1104).

## Example Usage
Make the following connections between the SPI Controller and SPI Peripheral:
- Controller SCLK -> Peripheral SCLK
- Controller PICO -> Peripheral PICO
- Controller POCI <- Peripheral POCI
- Controller CS   -> Peripheral CS

Compile, load and run the example. Ensure the SPI Peripheral example is
running on the device before the SPI Controller example.
Send bytes from SPI Controller to SPI Peripheral.
The LED will toggle after data is received.
The SPI is initialized with the following configuration:
- SPI Peripheral
- Motorola 4 Wire with Polarity 0, Phase 0
- No parity
- 8 bits per transfer
- MSB first
