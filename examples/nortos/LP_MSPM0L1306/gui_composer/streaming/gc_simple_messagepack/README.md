# Example Summary

This example demonstrates basic usage of how to send data using [MessagePack protocol](https://msgpack.org/index.html). In addition, it showcases how to connect target objects with GUI widgets. For details about the architecture of the example or about implementation, please view the User’s Guide.

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
| PA14 | GPIOA | PA14 | N/A | <ul><li>PA14 is connected to S2 button to GND with no external pull resistor<br><ul><li>Press `S2` button to connect pin to GND<br><li>Don't use `S2` button if not needed by application</ul></ul> |
| PA18 | GPIOA | PA18 | J3_26 | <ul><li>PA18 can be connected to S1 button to VCC with external pull-down<br><ul><li>`J11 ON` Connect S1 button and external pull-down<br><li>`J11 OFF` Disconnect S1 button and external pull-down</ul><br><li>PA18 can be connected to XDS-110 BSL_Invoke<br><ul><li>`J101 17:18 OFF` Disconnect pin to XDS-110 BSL Invoke</ul></ul> |
| PA9 | UART0 | RX | J1_3 | <ul><li>PA9 can be used as UART RX connected to XDS-110 backchannel or to boosterpack connector:<br><ul><li>To use backchannel UART on J101:<br>  `J17 1:2`: Connects XDS-110 backchannel UART RX to `J101`<br>  `J101 7:8` Connects XDS-110 backchannel to UART RX<br><li>To use UART on boosterpack connector:<br>  `J17 2:3`: Connects UART RX to `J1_3`</ul></ul> |
| PA8 | UART0 | TX | J1_4 | <ul><li>PA8 can be used as UART TX connected to XDS-110 backchannel or to boosterpack connector:<br><ul><li>To use backchannel UART on J101:<br>  `J16 1:2`: Connects XDS-110 backchannel UART TX to `J101`<br>  `J101 9:10` Connects XDS-110 backchannel to UART TX<br><li>To use UART on boosterpack connector:<br>  `J16 2:3`: Connects UART TX to` J1_4`</ul></ul> |
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

* Please read the Quick Start Guide on how to flash your LaunchPad with the code example, and how to import the provided GUI zip into GUI Composer.

- This example by default uses the **MSPM0_SimpleGUI_mpack.zip** GUI. This can be found [Online on the TI Gallery](https://dev.ti.com/gallery/view/TIMSPGC/MSPM0_SimpleGUI_mpack/)
- Alternatively, when optimizations are enabled in the application, the example can use the **MSPM0_SimpleGUI_mpack_optimized.zip** GUI. **To use this GUI, this requires changes to the default `gc_simple_mpack` application code.** Please read the User's Guide for more details. This GUI can be found [Online on the TI Gallery](https://dev.ti.com/gallery/view/TIMSPGC/MSPM0_SimpleGUI_mpack_optimized/)

* Run the example and the GUI.

The demo should start running with the following functionality:

- "Counters" plot shows an 8-bit, 16-bits and 32-bits counters

	![](../../../../../../docs/english/middleware/gui_composer/images/readme_counters.png)
	- The 8-bit counter corresponds to variable **u8Counter** and it's bound to token "**c1**". It increments by 50 when switch S2 is pressed.
	- The 16-bit counter corresponds to variable **u16Counter** and it's bound to token "**c2**". It increments by 5,000 when switch S1 is pressed.
	- The 32-bit counter corresponds to variable **u32Counter** and it's bound to token "**c3**". It increments by 10,000 periodically on Timer interrupt
- The gauge shows a floating point number, which in this case is sent as fractional (QMath/IQMath)

	![](../../../../../../docs/english/middleware/gui_composer/images/readme_gauge.png)
	- The counter corresponds to variable **qCounter** and it's bound to token "**c4**". It increments by **qIncrement** on Timer interrupt and only when bEnableSwitch==true.
- The increment for the gauge is set by the horizontal slider

	![](../../../../../../docs/english/middleware/gui_composer/images/readme_slider.png)
	- The increments corresponds to variable **qIncrement** and it's bound to token "**u16Data**". The value is sent by the GUI to the device when the slider changes.
- The "Enable Float" switch enables incrementing the gauge increments

	![](../../../../../../docs/english/middleware/gui_composer/images/readme_slider.png)
	- The switch corresponds to variable **bEnableSwitch** and it's bound to token "**bEnable**". The switch is sent by the GUI to the device when the value changes.

Notes:
- This example does not implement push-button debounce, so you might see multiple increments when pressing the button.
- The increments (50, 5000, 10000) are only meant to show the difference between variable type sizes.
- The example is sending ALL variables every time it wakes up due to button press or timer interrupt.
- It's recommended to keep the tokens short, especially when sending them often. Longer tokens will result in longer transmission times.

# Additional Information

For more information on modifying the GUI and serial communication, please view the [GUI Composer User's Guide](https://dev.ti.com/gc/v2/help/GC_UserGuide_v2/index.html).
