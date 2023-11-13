## Example Summary

Note: The use of "Master" and "Slave", along with "MOSI/MISO" terminology is
being considered obsolete. These terms will be replaced with "Controller" and
"Peripheral", and "PICO/POCI" respectively.

The following example configures the SPI as a Controller in internal loopback
mode to transmit and receive data.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| SPI0 | PA11 | SPI SCLK (Clock) |
| SPI0 | PA18 | SPI PICO (Peripheral In, Controller Out) |
| SPI0 | PA10 | SPI POCI (Peripheral Out, Controller In) |
| SPI0 | PA8 | SPI CS0 (Chip Select 0) |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L1306](https://www.ti.com/tool/LP-MSPM0L1306) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA11 | SPI0 | SCLK | J2_11/J4_35 | <ul><li>PA11 can be connected to two locations in boosterpack connector:<br><ul><li>`J14 1:2`: Connect pin to `J2_11`<br><li>`J14 2:3`: Connect pin to `J4_35`</ul></ul> |
| PA18 | SPI0 | MOSI | J3_26 | <ul><li>PA18 can be connected to S1 button to VCC with external pull-down<br><ul><li>`J11 ON` Connect S1 button and external pull-down<br><li>`J11 OFF` Disconnect S1 button and external pull-down</ul><br><li>PA18 can be connected to XDS-110 BSL_Invoke<br><ul><li>`J101 17:18 OFF` Disconnect pin to XDS-110 BSL Invoke</ul></ul> |
| PA10 | SPI0 | MISO | J4_36 | N/A |
| PA8 | SPI0 | CS0 | J1_4 | <ul><li>PA8 can be connected to XDS-110 backchannel or to boosterpack connector:<br><ul><li>To use on boosterpack connector and disconnect XDS-110:<br>  `J16 2:3`: Connect pin to `J1_4`</ul></ul> |
| PA20 | DEBUGSS | SWCLK | J2_13 | <ul><li>PA20 is used by SWD during debugging<br><ul><li>`J101 15:16 ON` Connect to XDS-110 SWCLK while debugging<br><li>`J101 15:16 OFF` Disconnect from XDS-110 SWCLK if using pin in application</ul></ul> |
| PA19 | DEBUGSS | SWDIO | J2_17 | <ul><li>PA19 is used by SWD during debugging<br><ul><li>`J101 13:14 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J101 13:14 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0L1306 User's Guide](https://www.ti.com/lit/slau869).

## Example Usage
Compile, load and run the example. The SPI will automatically start
to transmit and receive data.
The SPI is initialized with the following configuration:
- SPI Controller
- Motorola 4 Wire with Polarity 0, Phase 0
- No parity
- 8 bits per transfer
- MSB first
A SW breakpoint is set in the application to allow the user to check the value
of the received data.
