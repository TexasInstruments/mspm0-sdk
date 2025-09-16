## Example Summary

This example shows how to implement using MSPM0 to transmit/receive data to/from a CC110x device. It uses SPI communication and GPIO signals to communicate from one CC110X device to another CC110X device. The boards used for this example are the [EM Adapter Boosterpack](https://www.ti.com/tool/BOOST-CCEMADAPTER) and the [CC1101EM](https://www.ti.com/tool/CC1101-CC1190EM869RD). 

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOB | PB17 | Standard Output |
| GPIOB | PB27 | Standard Output |
| GPIOB | PB12 | Standard Input  |
| GPIOB | PB21 | Standard Input with internal pull-up |
| SYSCTL |  |  |
| SPI1 | PB9 | SPI SCLK (Clock) |
| SPI1 | PB8 | SPI PICO (Peripheral In, Controller Out) |
| SPI1 | PB7 | SPI POCI (Peripheral Out, Controller In) |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PB12 | GPIOB | PB17 | J4_18 | <ul><li>PB17 is used as the MSPM0 SPI CS <ul><li>Pin is controlled in code's transmit/receive functions</ul></ul> |
| PB27 | GPIOB | PB27 | J27_10 | <ul><li>PB27 can be connected to LED2 Green<br><ul><li>`J7 ON` Connect to LED2 Green<br><li>`J7 OFF` Disconnect from LED2 Green</ul></ul> |
| PB12 | GPIOB | PB12 | J4_19 | <ul><li>PB12 is used to receive a status signal from the CC110X <ul><li>Pin is controlled in code's transmit/receive functions</ul></ul> |
| PB21 | GPIOB | PB21 | J27_4 | <ul><li>PB21 is connected to S2 button to GND with no external pull resistor<br><ul><li>Press `S2` button to connect pin to GND<br><li>Don't use `S2` button if not needed by application</ul></ul> |
| PB9 | SPI1 | SCLK | J1_7 | N/A |
| PB8 | SPI1 | MOSI | J2_15 | N/A |
| PB7 | SPI1 | MISO | J2_14 | N/A |
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
Connect the CC1101EM to the EM Adapter Boosterpack (positioned so both EMs' labels can be read from the same angle), then connect the EM Adapter Boosterpack to the LP-MSPM0G3507. The connections with the MSPM0 device are as follows:
    SPI SCLK  --> LP1_7
    SPI_PICO  --> LP2_6
    SPI_POCI  --> LP2_7
    GPIO_CSn  --> LP2_3 
    GPIO_GDO0 --> LP2_2
To transmit data from the MSPMM0 to another CC110X, exclude the rfPacketRx.c from the project, run the project, press the S2 button to transmit the data, and press again to stop sending data. The LED should toggle when transmitting, and stop toggling when not. To receive data from another CC110X device, exclude the rfPacketTx.c from a project and run the project. If data is received correctly, the LED should toggle. 