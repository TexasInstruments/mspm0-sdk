## Example Summary

This example shows how to configure an I2C expander. The required data packet requires a command byte followed by a data byte: [CMD][DATA].

The commands to control the expander include:
- 0x1: GPIO direction set, where a 1 represents an output and a 0 represents an input in the DATA byte.
- 0x2: GPIO output control, where a 1 represents output HIGH and a 0 represents output LOW in the DATA byte.

A read request to the IO expander returns a byte containing the current state of all GPIO pins, regardless of direction.

When an input pin changes on the IO expander, the IRQ_OUT pin is pulled LOW until a read request is received. This signal is used to alert the controller to request a read from the IO expander.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOA | High-Speed Input | PA14 | J1_2 |  |
| GPIOA | High-Speed Input | PA9 | J1_3 |  |
| GPIOA | Standard Input | PA27 | J3_23 |  |
| GPIOA | Standard Input | PA8 | J1_4 |  |
| GPIOA | Standard Input | PA26 | J3_24 |  |
| GPIOA | Standard Input | PA15 | J3_30 |  |
| GPIOA | Standard with Wake Input | PA18 | J19_18 |  |
| GPIOA | Standard with Wake Input | PA18 | S1 | Populate Jumper(s): J1[1:2] |
| GPIOA | Open-Drain Input | PA0 | Red LED1 | Populate Jumper(s): J6[1:2] |
| GPIOA | High-Speed Output | PA12 | J4_32 |  |
| SYSCTL |  |  |  |  |
| I2C0 | I2C Serial Data line (SDA) | PA10 | J4_34 | Populate Jumper(s): J14[2:1] |
| I2C0 | I2C Serial Clock line (SCL) | PA11 | J4_33 | Populate Jumper(s): J15[2:1] |
| EVENT |  |  |  |  |
| BOARD | Debug Clock | PA20 | J19_16 |  |

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
MSPM0 LaunchPad, please visit the LP-MSPM0G3519 User's Guide.

## Example Usage

Compile, load and run the example. Connect the I2C SDA and SCL lines to the expander and ensure that the necessary Jumpers for pull-ups and/or LEDs are connected. Please refer to the related [app note](https://www.ti.com/lit/slaael4) for more details.
