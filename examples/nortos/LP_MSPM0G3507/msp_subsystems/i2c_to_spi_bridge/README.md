## Example Summary

The following example bridges incoming I2C transmissions to an SPI peripheral. The SPI is configured for 1MHz transmissions. The bridge receives data from the I2C input, sends that data through SPI, gets data from an SPI target, and then waits for an I2C Controller to request the data in the bridge. For more information, please read this project's subsystem design(https://www.ti.com/lit/sd/slaaes6/slaaes6.pdf).

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| I2C1 | PB3 | I2C Serial Data line (SDA) |
| I2C1 | PB2 | I2C Serial Clock line (SCL) |
| SPI1 | PA17 | SPI SCLK (Clock) |
| SPI1 | PA18 | SPI PICO (Peripheral In, Controller Out) |
| SPI1 | PA16 | SPI POCI (Peripheral Out, Controller In) |
| SPI1 | PA26 | SPI CS0 (Chip Select 0) |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PB3 | I2C1 | SDA | J1_10 | <ul><li>PB3 can be connected to an on-board pull-up resistor<br><ul><li>`R60` is not soldered by default<br><li>Solder `R60` to use on-board pull-up</ul></ul> |
| PB2 | I2C1 | SCL | J1_9 | <ul><li>PB2 can be connected to an on-board pull-up resistor<br><ul><li>`R59` is not soldered by default<br><li>Solder `R59` to use on-board pull-up</ul></ul> |
| PA17 | SPI1 | SCLK | J3_28 | N/A |
| PA18 | SPI1 | PICO | J2_26 | N/A |
| PA16 | SPI1 | POCI | J15_1 | N/A |
| PA26 | SPI1 | CS0  | J1_5  | N/A |
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
Connect SDA and SCL between the I2C Controller and the Bridge's I2C Target (remember to use pull-up resistors). Attach the SPI peripheral to the Bridge's SPI Controller. Compile, load, and run the example. After receiving an I2C message, the bridge translates it to SPI. Connect a logic analyzer in the I2C lines and SPI lines or check the buffer (gI2CRxPacket, gSPITxPacket, gSPIRxData) values after a transaction to verify the message is being sent correctly.
