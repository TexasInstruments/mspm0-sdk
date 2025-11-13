## Example Summary

Control and capture several GPIOs' status through UART, I2C or SPI.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOA | Standard Input | PA25 | J1_2 |  |
| GPIOA | Standard Input | PA8 | J1_10 |  |
| GPIOA | Standard Input | PA13 | J3_29 |  |
| GPIOA | Standard Input | PA29 | J2_12 |  |
| GPIOA | Standard Input | PA14 | J3_30 |  |
| GPIOA | Standard Input | PA16 | J3_26 |  |
| GPIOA | Standard Input | PA15 | J3_27 |  |
| SYSCTL |  |  |  |  |
| UC0 | I2C Serial Data line (SDA) | PA0 | J4_32 |  |
| UC0 | I2C Serial Clock line (SCL) | PA1 | J4_31 |  |
| UC1 | RX Pin | PA31 | J4_35 |  |
| UC1 | TX Pin | PA28 | J4_36 |  |
| UC2 | SPI SCLK (Clock) | PA11 | J4_33 |  |
| UC2 | SPI PICO (Peripheral In, Controller Out) | PA9 | J1_9 |  |
| UC2 | SPI POCI (Peripheral Out, Controller In) | PA10 | J4_34 |  |
| EVENT |  |  |  |  |
| BOARD | Debug Clock | PA20 | J101_16 |  |
| BOARD | Debug Data In Out | PA19 | J101_14 |  |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0C1104 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0C1104 User's Guide](https://www.ti.com/lit/slau908).

## Example Usage

Compile, load and run the example. Then you can control and capture several GPIOs' status through UART, I2C or SPI. Please refer to the related [app note](https://www.ti.com/lit/slaael9) for more details.
