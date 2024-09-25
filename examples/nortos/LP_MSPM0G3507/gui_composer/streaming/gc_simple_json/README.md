# Example Summary

This example demonstrates basic usage of how to send data using [JSON formatted strings](https://www.json.org/json-en.html). In addition, it showcases how to connect target objects with GUI widgets. For details about the architecture of the example or about implementation, please view the User’s Guide.

## Peripherals & Pin Assignments
- UART0
    + UART0_RX / PA11
    + UART0_TX / PA10
- GPIOA
    + S1 / PA18
- GPIOB
    + S2 / PB21

## BoosterPacks, Board Resources & Jumper Settings
- Boosterpack connections:
    + J2.34 - PA10 (GPIO_UART_TX_PIN)
    + J2.33 - PA11 (GPIO_UART_RX_PIN)
- Board resources:
    + XDS110 - Back-channel UART
- Jumper settings:
    + J25 [1:2]
    + J26 [1:2]
    + J101 [7:8]
    + J101 [9:10]


### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0G3507 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#non-sysconfig-compatible-project-migration)
for information about migrating to other MSPM0 devices.

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
