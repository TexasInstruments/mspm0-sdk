## Example Summary

This example demontrates an MSPM0 I2C Target configured as an IO expander.  An MSPM0 I2C Controller uses a simple protocol to 'set', 'clear', 'toggle' and 'read' the target GPIOs.<br>
A specific packet format is expected for proper operation consisting of 6 bytes: <br>
I2C data packet: [PORT] [REGISTER] [B3] [B2] [B1] [B0]<br>
   - [PORT] represents the IO expander port, such as PORTA, PORTB, PORT, etc.
   - [REGISTER] represents the SET, CLEAR, TOGGLE and DIN registers.
   - [B3] [B2] [B1] [B0] is a 32-bit word with [B3] = MSB and [B0] = LSB and represents the bits that will be acted on.<br>

Please see accompanying sub-system document for more information. [I2C IO Exander Sub-system](https://www.ti.com/lit/????)

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOA | High-Speed Input with internal pull-down | PA13 | J4_31 |  |
| GPIOA | Standard with Wake Input | PA18 | S1 | Populate Jumper(s): J1[1:2] |
| GPIOA | Open-Drain Output | PA0 | Red LED1 | Populate Jumper(s): J6[1:2] |
| GPIOB | Standard Output | PB26 | LED2 Red | Populate Jumper(s): J8[1:2] |
| GPIOB | Standard Output | PB27 | LED2 Green | Populate Jumper(s): J9[1:2] |
| GPIOB | Standard Output | PB22 | LED2 Blue | Populate Jumper(s): J7[1:2] |
| GPIOB | Standard Input with internal pull-up | PB21 | J7_26 |  |
| GPIOB | Standard Output with internal pull-up | PB0 | J5_2 |  |
| GPIOB | Standard Output | PB4 | J4_36 |  |
| SYSCTL |  |  |  |  |
| TIMG0 |  |  |  |  |
| I2C1 | I2C Serial Data line (SDA) | PB3 | J4_40 |  |
| I2C1 | I2C Serial Clock line (SCL) | PB2 | J6_40 |  |
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
Make connections to target MSPM0 as described above.
<br>Compile, load and run the example.</br>
On the MSPM0 Controller, press the USER button to step through the following sequence:
<ul><li>Press -> command Target to <u>set</u> (turn on) LED_RGB_GREEN</li>
<li>Press -> command Target to <u>clear</u> (turn off) LED_RGB_GREEN</li>
<li>Press -> command Target to <u>toggle</u> (turn on) LED_RGB_RED</li>
<li>Press -> command Target to <u>toggle</u> (turn off) LED_RGB_LED</li>
<li>Press -> command Target to read PORTx<br>
<ul><li>On the MSPM0 Target, press and hold BSL Button first</li>
<li>If return value is logic '1' (Target BSL Button pressed), command Target to <u>set</u> (turn on) Target LED_RGB_BLUE</li>
<li>else LED_RGB_BLUE remains off</li></ul>
<li>Press -> repeat sequence</li></ul>
   
    
