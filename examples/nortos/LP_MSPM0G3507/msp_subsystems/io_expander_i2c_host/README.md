## Example Summary

This example demontrates an MSPM0 host I2C controller exercising another MSPM0 I2C target configured as an IO expander using a simple protocol to 'set', 'clear', 'toggle' and 'read' the target GPIOs.<br>
A specific packet format is expected for proper operation consisting of 6 bytes: <br>
I2C data packet: [PORT] [REGISTER] [B3] [B2] [B1] [B0]<br>
   - [PORT] represents the IO expander port, such as PORTA, PORTB, PORT, etc.
   - [REGISTER] represents the SET, CLEAR, TOGGLE and DIN registers.
   - [B3] [B2] [B1] [B0] is a 32-bit word with [B3] = MSB and [B0] = LSB and represents the bits that will be acted on.<br>

Please see accompanying sub-system document for more information. [I2C IO Exander Sub-system](https://www.ti.com/lit/????)

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA18 | Standard with Wake Input |
| GPIOA | PA0 | Open-Drain Output |
| GPIOB | PB27 | Standard Output |
| GPIOB | PB26 | Standard Output |
| GPIOB | PB22 | Standard Output |
| GPIOB | PB21 | Standard Input with internal pull-up |
| GPIOB | PB0 | Standard Input with internal pull-up |
| SYSCTL |  |  |
| TIMG0 |  |  |
| TIMG6 |  |  |
| I2C1 | PB3 | I2C Serial Data line (SDA) |
| I2C1 | PB2 | I2C Serial Clock line (SCL) |
| EVENT |  |  |
| CRC |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA18 | GPIOA | PA18 | J3_26/J3_29 | <ul><li>PA18 can be connected to S1 button to VCC with external pull-down<br><ul><li>`J8 ON` Connect S1 button and external pull-down<br><li>`J8 OFF` Disconnect S1 button and external pull-down</ul><br><li>PA18 can be connected to XDS-110 BSL_Invoke<br><ul><li>`J101 17:18 OFF` Disconnect pin to XDS-110 BSL Invoke</ul><br><li>PA18 can be connected to boosterpack pin J3_29<br><ul><li>`J15 2:3` Connect pin to J3_29<br><li>`J15 1:2/OFF` Disconnect pin from J3_29</ul><br><li>PA18 can be connected to boosterpack pin J3_26<br><ul><li>`R58` is soldered and connects pin to J3_26 by default</ul></ul> |
| PA0 | GPIOA | PA0 | J27_9 | <ul><li>PA0 is 5V tolerant open-drain so it requires pull-up<br><ul><li>`J19 1:2` Use 3.3V pull-up<br><li>`J19 2:3` Use 5V pull-up</ul><br><li>PA0 can be connected to LED1<br><ul><li>`J4 ON` Connect to LED1<br><li>`J4 OFF` Disconnect from LED1</ul></ul> |
| PB27 | GPIOB | PB27 | J27_10 | <ul><li>PB27 can be connected to LED2 Green<br><ul><li>`J7 ON` Connect to LED2 Green<br><li>`J7 OFF` Disconnect from LED2 Green</ul></ul> |
| PB26 | GPIOB | PB26 | J27_8 | <ul><li>PB26 can be connected to LED2 Red<br><ul><li>`J6 ON` Connect to LED2 Red<br><li>`J6 OFF` Disconnect from LED2 Red</ul></ul> |
| PB22 | GPIOB | PB22 | J27_5 | <ul><li>PB22 can be connected to LED2 Blue<br><ul><li>`J5 ON` Connect to LED2 Blue<br><li>`J15 OFF` Disconnect from LED2 Blue</ul></ul> |
| PB21 | GPIOB | PB21 | J27_4 | <ul><li>PB21 is connected to S2 button to GND with no external pull resistor<br><ul><li>Press `S2` button to connect pin to GND<br><li>Don't use `S2` button if not needed by application</ul></ul> |
| PB0 | GPIOB | PB0 | J2_12 | N/A |
| PB3 | I2C1 | SDA | J1_10 | <ul><li>PB3 can be connected to an on-board pull-up resistor<br><ul><li>`R60` is not soldered by default<br><li>Solder `R60` to use on-board pull-up</ul></ul> |
| PB2 | I2C1 | SCL | J1_9 | <ul><li>PB2 can be connected to an on-board pull-up resistor<br><ul><li>`R59` is not soldered by default<br><li>Solder `R59` to use on-board pull-up</ul></ul> |
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
   
    
