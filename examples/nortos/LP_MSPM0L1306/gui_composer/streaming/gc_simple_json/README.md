# Example Summary

This example demonstrates basic usage of how to send data using [JSON formatted strings](https://www.json.org/json-en.html). In addition, it showcases how to connect target objects with GUI widgets. For details about the architecture of the example or about implementation, please view the User’s Guide.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA14 | Standard Input with internal pull-up |
| GPIOA | PA18 | Standard with Wake Input |
| SYSCTL |  |  |
| TIMG0 |  |  |
| UART0 | PA9 | RX Pin |
| UART0 | PA8 | TX Pin |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L1306](https://www.ti.com/tool/LP-MSPM0L1306) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA14 | GPIOA | PA14 | N/A | Connected to S2 button (no pull resistor) |
| PA18 | GPIOA | PA18 | J3_26 | J11 ON: Connect S1 button (with pull-down)<br>J101 17:18 OFF: Disconnect from XDS-110 BSL_Invoke |
| PA9 | UART0 | RX | J1_3 | J17 1:2 Connect XDS-110 BackChannel UART on J101<br>J17 2:3 Connect to J1_3<br>J101 7:8 Connect XDS-110 BackChannel UART RX |
| PA8 | UART0 | TX | J1_4 | J16 1:2 Connect XDS-110 BackChannel UART on J101<br>J16 2:3 Connect to J1_4<br>J101 9:10 Connect XDS-110 BackChannel UART TX |
| PA20 | DEBUGSS | SWCLK | J2_13 | J101 15:16 ON: Connect to XDS-110 SWCLK (debug) |
| PA19 | DEBUGSS | SWDIO | J2_17 | J101 13:14 ON: Connect to XDS-110 SWDIO (debug) |

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0L1306 User's Guide](https://www.ti.com/lit/slau869).

## Example Usage

* Please read the Quick Start Guide on how to flash your LaunchPad with the code example, and how to import the provided GUI zip into GUI Composer.

- This example uses the **MSPM0_SimpleGUI_JSON.zip** GUI. This can be found [Online on the TI Gallery](https://dev.ti.com/gallery/view/TIMSPGC/MSPM0_SimpleGUI_JSON/)

* Run the example and the GUI.

The demo should start running with the following functionality:

- "Counters" plot shows an 8-bit, 16-bits and 32-bits counters

	![](../../../../../../docs/english/middleware/gui_composer/images/readme_counters.png)
	- The 8-bit counter corresponds to variable **u8Counter** and it's bound to JSON token "**c1**". It increments by 50 when switch S2 is pressed.
	- The 16-bit counter corresponds to variable **u16Counter** and it's bound to JSON token "**c2**". It increments by 5,000 when switch S1 is pressed.
	- The 32-bit counter corresponds to variable **u32Counter** and it's bound to JSON token "**c3**". It increments by 10,000 periodically on Timer interrupt
- The gauge shows a floating point number, which in this case is sent as fractional (QMath/IQMath)

	![](../../../../../../docs/english/middleware/gui_composer/images/readme_gauge.png)
	- The counter corresponds to variable **qCounter** and it's bound to JSON token "**c4**". It increments by **qIncrement** on Timer interrupt and only when bEnableSwitch==true.
- The increment for the gauge is set by the horizontal slider

	![](../../../../../../docs/english/middleware/gui_composer/images/readme_slider.png)
	- The increments corresponds to variable **qIncrement** and it's bound to JSON token "**u16Data**". The value is sent by the GUI to the device when the slider changes.
- The "Enable Float" switch enables incrementing the gauge increments

	![](../../../../../../docs/english/middleware/gui_composer/images/readme_slider.png)
	- The switch corresponds to variable **bEnableSwitch** and it's bound to JSON token "**bEnable**". The switch is sent by the GUI to the device when the value changes.

Notes:
- This example does not implement push-button debounce, so you might see multiple increments when pressing the button.
- The increments (50, 5000, 10000) are only meant to show the difference between variable type sizes.
- The example is sending ALL variables every time it wakes up due to button press or timer interrupt.
- It's recommended to keep the JSON tokens short, especially when sending them often. Longer tokens will result in longer transmission times.

# Additional Information

For more information on modifying the GUI and serial communication, please view the [GUI Composer User's Guide](https://dev.ti.com/gc/v2/help/GC_UserGuide_v2/index.html).