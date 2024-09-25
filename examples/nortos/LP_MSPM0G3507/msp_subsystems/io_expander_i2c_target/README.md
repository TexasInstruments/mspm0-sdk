## Example Summary

This example demontrates an MSPM0 I2C Target configured as an IO expander.  An MSPM0 I2C Controller uses a simple protocol to 'set', 'clear', 'toggle' and 'read' the target GPIOs.<br>
A specific packet format is expected for proper operation consisting of 6 bytes: <br>
I2C data packet: [PORT] [REGISTER] [B3] [B2] [B1] [B0]<br>
   - [PORT] represents the IO expander port, such as PORTA, PORTB, PORT, etc.
   - [REGISTER] represents the SET, CLEAR, TOGGLE and DIN registers.
   - [B3] [B2] [B1] [B0] is a 32-bit word with [B3] = MSB and [B0] = LSB and represents the bits that will be acted on.<br>

Please see accompanying sub-system document for more information. [I2C IO Exander Sub-system](https://www.ti.com/lit/????)

## Peripherals & Pin Assignments Used in this example

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL | --- | --- |
| TIMG0 | --- | Blocking Delay |
| TIMG6 | --- | Non-Blocking Delay |
| I2C1 | PB3 | I2C Serial Data line (SDA) |
| I2C1 | PB2 | I2C Serial Clock line (SCL) |
| GPIO | PB0 | IRQ_OUT |
| GPIO | PB22 | LED_RGB_BLUE |
| GPIO | PB26 | LED_RGB_RED |
| GPIO | PB27 | LED_RGB_GREEN |
| GPIO | PA18 | SW_2 - BSL Button|
| GPIO | PB21 | SW_3 - User Button|
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PB3 | I2C1 | SDA | J1_10 | <ul><li>PB3 Jumper to target MCU I2C-SDA</ul></ul> |
| PB2 | I2C1 | SCL | J1_9 | <ul><li>PB2 Jumper to target MCU I2C-SCL</ul></ul> |
| PB0 | GPIO | OUTPUT | J2_12 | <ul><li>Interrupt Output Pin<br><ul><li>This pin is configured as output<br><li>Jumper to target MCU IO Expander interrupt output pin.  See target demo for details.<br></ul></ul>|
| PB22 | GPIO | OUTPUT | J16 | <ul><li>BLUE RGB LED<br><ul></ul> |
| PB26 | GPIO | OUTPUT | J18 | <ul><li>RED RGB LED<br><ul></ul> |
| PB27 | GPIO | OUTPUT | J17 | <ul><li>GREEN RGB LED<br><ul></ul> |
| PA18 | GPIO | INPUT | J3_26 | <ul><li>SW_2 (BSL Button)<br><ul><li>This pin is configured as input</ul>|
| PB21 | GPIO | INPUT | --- | <ul><li>SW_3 (USER Button)<br><ul><li>This pin is configured as input with pull up resistors enabled</ul>|
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
   
    
