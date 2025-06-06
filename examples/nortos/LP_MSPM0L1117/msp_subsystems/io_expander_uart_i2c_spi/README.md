## Example Summary

Control and capture several GPIOs' status through UART, I2C or SPI.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOA | Standard Input | PA8 | J1_4 |  |
| GPIOA | High-Speed Input | PA9 | J1_3 |  |
| GPIOA | Standard Input | PA25 | J1_2 |  |
| GPIOA | Standard Input | PA26 | J1_5 |  |
| GPIOA | Standard Input | PA14 | J1_6 |  |
| GPIOA | High-Drive Input | PA18 | S1 | Populate Jumper(s): J2[1:2] |
| GPIOA | Standard Input | PA24 | J3_27 |  |
| GPIOA | High-Drive Input | PA17 | J3_28 |  |
| SYSCTL |  |  |  |  |
| I2C0 | I2C Serial Data line (SDA) | PA0 | J1_10 |  |
| I2C0 | I2C Serial Clock line (SCL) | PA1 | J1_9 |  |
| UART0 | RX Pin | PA11 | J4_33 | Populate Jumper(s): J12[2:3] |
| UART0 | TX Pin | PA10 | J4_34 | Populate Jumper(s): J10[2:3] |
| SPI0 | SPI SCLK (Clock) | PA12 | J3_25 |  |
| SPI0 | SPI PICO (Peripheral In, Controller Out) | PA5 | J21_1 | Populate Component(s): J21 |
| SPI0 | SPI POCI (Peripheral Out, Controller In) | PA13 | J2_14 |  |
| EVENT |  |  |  |  |
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

Compile, load and run the example. Then you can control and capture several GPIOs' status through UART, I2C or SPI. Please refer to the related [app note](https://www.ti.com/lit/slaael9) for more details.
