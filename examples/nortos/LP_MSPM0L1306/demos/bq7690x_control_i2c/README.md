## Example Summary

This code includes many functions for using the different commands for the BQ7690x
family, shows how to configure the device settings, and demonstrates a simple measurement loop.
The code then continuously flashes the LP-MSPM0L1306 on-board LEDs to show the code completion.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA26 | Standard Output |
| GPIOA | PA27 | Standard Output |
| GPIOA | PA13 | Standard Output |
| SYSCTL |  |  |
| I2C0 | PA0 | I2C Serial Data line (SDA) |
| I2C0 | PA1 | I2C Serial Clock line (SCL) |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L1306](https://www.ti.com/tool/LP-MSPM0L1306) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA26 | GPIOA | PA26 | J4_38 | <ul><li>PA26 can be connected to LED2 Red<br><ul><li>`J12 ON` Connect to LED2 Red<br><li>`J12 OFF` Disconnect from LED2 Red</ul></ul> |
| PA27 | GPIOA | PA27 | J4_37 | <ul><li>PA27 can be connected to LED2 Blue<br><ul><li>`J13 ON` Connect to LED2 Blue<br><li>`J13 OFF` Disconnect from LED2 Blue</ul></ul> |
| PA13 | GPIOA | PA13 | J4_39 | <ul><li>PA13 can be connected to LED2 Green<br><ul><li>`J3 ON` Connect to LED2 Green<br><li>`J3 OFF` Disconnect from LED2 Green</ul></ul> |
| PA0 | I2C0 | SDA | J1_10 | <ul><li>PA0 is 5V tolerant open-drain so it requires pull-up<br><ul><li>`J10 2:3` Use 3.3V pull-up<br><li>`J10 1:2` Use 5V pull-up</ul><br><li>PA0 can be connected to LED1<br><ul><li>`J2 OFF` Disconnect from LED1</ul></ul> |
| PA1 | I2C0 | SCL | J1_9 | <ul><li>PA1 is 5V tolerant open-drain so it requires pull-up<br><ul><li>`J19 2:3` Use 3.3V pull-up<br><li>`J9 1:2` Use 5V pull-up</ul></ul> |
| PA20 | DEBUGSS | SWCLK | J2_13 | <ul><li>PA20 is used by SWD during debugging<br><ul><li>`J101 15:16 ON` Connect to XDS-110 SWCLK while debugging<br><li>`J101 15:16 OFF` Disconnect from XDS-110 SWCLK if using pin in application</ul></ul> |
| PA19 | DEBUGSS | SWDIO | J2_17 | <ul><li>PA19 is used by SWD during debugging<br><ul><li>`J101 13:14 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J101 13:14 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0L1306 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0L1306 User's Guide](https://www.ti.com/lit/slau869).

## Example Usage

Connect LP-MSPM0L1306 to BQ76907EVM as outlined in following sections.

Import, build and run example on LP-MSPM0L1306.
A terminal in the debugger will display a message to show start of execution:

    Starting Test Sequence with BQ7690x..

If the I2C connection to BQ769x is successful, the terminal will display results
from reading and writing registers, resetting the device, reading voltages, currents,
and temperature.

LEDs will toggle at the end of the tests if the connection was successful.


### LP-MSPM0L1306 Pin Connections

Connection description: The I2C SCL and SDA pins are the only pin connections required between the
MSPM0 board and the BQ7690x for this demo code. Also a ground connection should be made between the 2 devices.

                             /|\ /|\
         MSPM0             5k |   |
         -----------------    |  5k
        |         I2C_SCL |---+---|-- I2C Clock (SCL)
        |                 |       |
        |         I2C_SDA |-------+-- I2C Data (SDA)
        |                 |
     ---| P           GND |---------- Common GND
        |                 |
     ---| P        		  |
        |                 |
     ---| P               |
        |                 |

Visit [LP_MSPM0L1306](https://www.ti.com/tool/LP-MSPM0L1306) for LaunchPad information, including user guide and hardware files.

### BQ7690x/BQ76907EVM Pin Connections

Connection description: The I2C SCL and SDA pins are the only pin connections required between the
MSPM0 board and the BQ7690x for this demo code. Also a ground connection should be made between the 2 devices.

                             /|\ /|\
         BQ7690x           5k |   |
         -----------------    |  5k
        |         SCL(12) |---+---|-- I2C Clock (SCL) ---> to MSPM0 I2C_SCL
        |                 |       |
        |         SDA(13) |-------+-- I2C Data (SDA)  ---> to MSPM0 I2C_SDA
        |                 |
        |         VSS(11) |---------- Common GND      ---> to MSPM0 GND
        |                 |
        |                 |
        |                 |
        |                 |
        |                 |


                             /|\ /|\
         BQ76907EVM        5k |   |
         -----------------    |  5k
        |       SCL(J4-2) |---+---|-- I2C Clock (SCL) ---> to MSPM0 I2C_SCL
        |                 |       |
        |       SDA(J4-3) |-------+-- I2C Data (SDA)  ---> to MSPM0 I2C_SDA
        |                 |
        |       VSS(J4-1) |---------- Common GND      ---> to MSPM0 GND
        |                 |
        |                 |
        |                 |
        |                 |
        |                 |

The I2C Clock and I2C Data lines should both have one pull-up on each line. The BQ76907EVM includes pullups to REGOUT which can be enabled by installing shunts J1 and J5 on the BQ76907EVM, marked Pullups to REGOUT. The LP-MSPM0L1306 board also includes 5V and 3V pullup shunts located at J9 and J10 on the LP-MSPM0L1306 board.

Visit [BQ76907](https://www.ti.com/product/BQ76907) for device information, including user guide and hardware files.
