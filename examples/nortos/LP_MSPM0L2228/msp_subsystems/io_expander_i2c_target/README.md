## Example Summary

This example demontrates an MSPM0 I2C Target configured as an IO expander.  An MSPM0 I2C Controller uses a simple protocol to 'set', 'clear', 'toggle' and 'read' the target GPIOs.<br>
A specific packet format is expected for proper operation consisting of 6 bytes: <br>
I2C data packet: [PORT] [REGISTER] [B3] [B2] [B1] [B0]<br>
   - [PORT] represents the IO expander port, such as PORTA, PORTB, PORT, etc.
   - [REGISTER] represents the SET, CLEAR, TOGGLE and DIN registers.
   - [B3] [B2] [B1] [B0] is a 32-bit word with [B3] = MSB and [B0] = LSB and represents the bits that will be acted on.<br>

Please see accompanying sub-system document for more information. [I2C IO Exander Sub-system](https://www.ti.com/lit/????)

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA13 | Standard Input with internal pull-down |
| GPIOA | PA18 | Standard with Wake Input |
| GPIOA | PA10 | High-Drive Output |
| GPIOA | PA23 | Standard Output |
| GPIOB | PB10 | Standard Output |
| GPIOB | PB9 | Standard Output |
| GPIOB | PB8 | Standard Input with internal pull-up |
| GPIOB | PB0 | Standard Output with internal pull-up |
| GPIOB | PB4 | Standard Output |
| SYSCTL |  |  |
| TIMG0 |  |  |
| I2C0 | PA0 | I2C Serial Data line (SDA) |
| I2C0 | PA1 | I2C Serial Clock line (SCL) |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L2228](https://www.ti.com/tool/LP-MSPM0L2228) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA13 | GPIOA | PA13 | J3_27 | <ul><li>PA13 has a capacitor to be used for LCDCAP0 functionality by default in Launchpad<br><ul><li>Remove `C50` if needed to use pin for other functions</ul></ul> |
| PA18 | GPIOA | PA18 | J3_23 | <ul><li>PA18 can be connected to S2 button to VCC with external pull-down<br><ul><li>`J1 ON` Connect S2 button and external pull-down<br><li>`J1 OFF` Disconnect S2 button and external pull-down</ul><br><li>PA18 can be connected to XDS-110 BSL_Invoke<br><ul><li>`J14 17:18 OFF` Disconnect pin to XDS-110 BSL Invoke</ul></ul> |
| PA10 | GPIOA | PA10 | J3_30 | <ul><li>PA10 can be connected to XDS-110 backchannel or boosterpack connector:<br><ul><li>To use on boosterpack connector J3_30 and disconnect XDS-110:<br> `J14 9:10 OFF` Disconnects XDS-110 backchannel to UART TX</ul></ul> |
| PA23 | GPIOA | PA23 | N/A | <ul><li>PA23 can be connected to VCC using R8 or to GND using C9<br><ul><li>`R8` and `C9` are not soldered by default allowing the pin to be used for other functions</ul><br><li>PA23 can be connected to LED4 Blue<br><ul><li>`J4 ON` Connect to LED4 Blue<br><li>`J4 OFF` Disconnect from LED4 Blue</ul></ul> |
| PB10 | GPIOB | PB10 | N/A | <ul><li>PB10 can be connected to LED4 Red<br><ul><li>`J5 ON` Connect to LED4 Red<br><li>`J5 OFF` Disconnect from LED4 Red</ul></ul> |
| PB9 | GPIOB | PB9 | N/A | <ul><li>PB9 can be connected to LED4 Green<br><ul><li>`J6 ON` Connect to LED4 Green<br><li>`J6 OFF` Disconnect from LED4 Green</ul></ul> |
| PB8 | GPIOB | PB8 | J7_15 | <ul><li>PB8 is connected to S3 button to GND with no external pull resistor<br><ul><li>Press `S3` button to connect pin to GND<br><li>Don't use `S3` button if not needed by application</ul></ul> |
| PB0 | GPIOB | PB0 | J7_37 | N/A |
| PB4 | GPIOB | PB4 | J7_11 | N/A |
| PA0 | I2C0 | SDA | J3_10 | <ul><li>PA0 is 5V tolerant open-drain so it requires pull-up<br><ul><li>`J12 1:2` Use 3.3V pull-up<br><li>`J12 2:3` Use 5V pull-up</ul><br><li>PA0 can be connected to LED3<br><ul><li>`J2 OFF` Disconnect from LED3</ul></ul> |
| PA1 | I2C0 | SCL | J3_9 | <ul><li>PA1 is 5V tolerant open-drain so it requires pull-up<br><ul><li>`J13 1:2` Use 3.3V pull-up<br><li>`J13 2:3` Use 5V pull-up</ul></ul> |
| PA20 | DEBUGSS | SWCLK | N/A | <ul><li>PA20 is used by SWD during debugging<br><ul><li>`J14 15:16 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J14 15:16 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |
| PA19 | DEBUGSS | SWDIO | N/A | <ul><li>PA19 is used by SWD during debugging<br><ul><li>`J14 13:14 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J14 13:14 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0L2228 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0L2228 web page](https://www.ti.com/tool/LP-MSPM0L2228).

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
   
    
