# Example Summary

This example demonstrates basic usage of how to send data using [JSON formatted strings](https://www.json.org/json-en.html). In addition, it showcases how to connect target objects with GUI widgets. For details about the architecture of the example or about implementation, please view the User’s Guide.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | Standard Input with internal pull-up | PA7 | S1 |
| GPIOA | Standard with Wake Input | PA18 | S2 |
| SYSCTL |  |  |
| TIMG0 |  |  |
| UC0 | RX Pin | PA11 |
| UC0 | TX Pin | PA10 |
| BOARD | Debug Clock | PA20 | J19_16 |
| BOARD | Debug Data In Out | PA19 | J19_14 |

### Device Migration Recommendations
This project was developed for a superset device included in the MSPM0 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0G5187 User's Guide](https://www.ti.com/lit/slau967).

## Example Usage

* Please read the Quick Start Guide on how to flash your LaunchPad with the code example, and how to import the provided GUI zip into GUI Composer.

- This example uses the **MSPM0_SimpleGUI_JSON.zip** GUI. This can be found [Online on the TI Gallery](https://dev.ti.com/gallery/view/TIMSPGC/MSPM0_SimpleGUI_JSON/)

* Run the example and the GUI.

The demo should start running with the following functionality:

- "Counters" plot shows an 8-bit, 16-bits and 32-bits counters

	![](../../../../../docs/english/middleware/gui_composer/images/readme_counters.png)
	- The 8-bit counter corresponds to variable **u8Counter** and it's bound to JSON token "**c1**". It increments by 50 when switch S2 is pressed.
	- The 16-bit counter corresponds to variable **u16Counter** and it's bound to JSON token "**c2**". It increments by 5,000 when switch S1 is pressed.
	- The 32-bit counter corresponds to variable **u32Counter** and it's bound to JSON token "**c3**". It increments by 10,000 periodically on Timer interrupt
- The gauge shows a floating point number, which in this case is sent as fractional (QMath/IQMath)

	![](../../../../../docs/english/middleware/gui_composer/images/readme_gauge.png)
	- The counter corresponds to variable **qCounter** and it's bound to JSON token "**c4**". It increments by **qIncrement** on Timer interrupt and only when bEnableSwitch==true.
- The increment for the gauge is set by the horizontal slider

	![](../../../../../docs/english/middleware/gui_composer/images/readme_slider.png)
	- The increments corresponds to variable **qIncrement** and it's bound to JSON token "**u16Data**". The value is sent by the GUI to the device when the slider changes.
- The "Enable Float" switch enables incrementing the gauge increments

	![](../../../../../docs/english/middleware/gui_composer/images/readme_slider.png)
	- The switch corresponds to variable **bEnableSwitch** and it's bound to JSON token "**bEnable**". The switch is sent by the GUI to the device when the value changes.

Notes:
- This example does not implement push-button debounce, so you might see multiple increments when pressing the button.
- The increments (50, 5000, 10000) are only meant to show the difference between variable type sizes.
- The example is sending ALL variables every time it wakes up due to button press or timer interrupt.
- It's recommended to keep the JSON tokens short, especially when sending them often. Longer tokens will result in longer transmission times.

# Additional Information

For more information on modifying the GUI and serial communication, please view the [GUI Composer User's Guide](https://dev.ti.com/gc/v2/help/GC_UserGuide_v2/index.html).
