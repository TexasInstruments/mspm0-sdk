## Out of Box Experience Summary

The out of box demo is a simple demo that implements various examples that
showcase the analog functionality of the MSPM0L1306 LaunchPad.
A GUI can be used with this demo, and the user can interact with the
LaunchPad using the GUI. The GUI for this demo can be found
[here](https://dev.ti.com/gallery/view/TIMSPGC/LP_MSPM0L1306_OOBE/).

Please contact your local sales representative or TI support team to request access
to the GUI.

### Demo Examples

#### Blinking LED Summary

Toggles LED2, the RGB LED, at a frequency that is configurable by the
accompanying GUI. LED2 will be green and will toggle every 500ms by default.

#### Light Sensor Summary

Uses OPA0 in general purpose mode along with the on-board photodiode,
capacitor, and resistor to implement a light sensor circuit.
The resulting voltage is measured by the ADC0 module.
The ADC result is used to control TimerG1's and TimerG0's PWMs
duty-cycles to modulate the brightness of the RGB LED. The ADC result will
also be displayed on the GUI.

#### Thermistor Summary

Uses OPA0 in buffer mode along with the on-board thermistor to implement a
temperature sensing circuit. The resulting voltage is measured by the internal
ADC0 module. The ADC result is used to set the color of the RGB LED.
Temperature drops of over 2 degrees Celcius will result in a blue LED while
temperature increases of over 2 degrees Celcius will result in a red LED.
Temperature changes less than 2 degrees Celcius will result in a green LED.

## Demo Usage

* Open a serial session (e.g. [`PuTTY`](http://www.putty.org/ "PuTTY's
Homepage"), etc.) to the appropriate COM port.
    * The COM port can be determined via Device Manager in Windows or via
`ls /dev/tty*` in Linux.

The connection should have the following settings
```
    Baud-rate:  115200
    Data bits:       8
    Stop bits:       1
    Parity:       None
    Flow Control: None
```

* Run the demo. As mentioned above, the demo implements several examples. Use S2
on the LaunchPad or interact with the GUI to switch between examples. The specific
example and their usage will be listed below.

#### Blinking LED Usage

In the GUI, click "Enable Blink LED" to start the example. Drag the slider to adjust the
LED toggle rate. Click "Enable Blink LED" again to stop the example and return
to the default IDLE mode where LED1 will toggle every 500ms.

##### Peripherals & Pin Assignments

- TIMG4
- GPIOA
    + Input with internal pull-up: PA14
    + Output: PA13

##### BoosterPacks, Board Resources & Jumper Settings

- Boosterpack connections:
    + J2 - PA0
- Board resources:
    + LED2.Green - PA13 (GPIO_RGB_GREEN_PIN)
    + S2 - PA14 (GPIO_SWITCHES_USER_SWITCH_1_PIN)
- Jumper settings:
    + J3 ON

#### Light Sensor Usage

In the GUI, click "Enable Light Sensor" to start the example. Increase or decrease the
amount of light hitting the light sensor and notice the RGB LED brightening or dimming.
Click "Enable Light Sensor" again to stop the example and return to the default
IDLE mode where LED1 toggles every 500ms.

##### Peripherals & Pin Assignments

- GPIOA
    + Input with internal pull-up: PA14
    + Output: PA27
	+ Output: PA26
	+ Output: PA13
- TIMG2
- OPA0
    + OPA0_IN0+ / PA25
    + OPA0_IN0- / PA24
    + OPA0_OUT / PA22
- ADC0
    + ADC0_4 / PA22

##### BoosterPacks, Board Resources & Jumper Settings

- Boosterpack connections:
    + J2 - PA0
- Board resources:
    + S2 - PA14 (GPIO_SWITCHES_USER_SWITCH_1_PIN)
    + LED2.Blue - PA27 (GPIO_RGB_BLUE_PIN)
    + LED2.Red - PA26 (GPIO_RGB_RED_PIN)
    + LED2.Green - PA13 (GPIO_RGB_GREEN_PIN)
- Jumper settings:
    + J13 ON
    + J12 ON
    + J3 ON
    + J4 ON
    + J5 ON
    + J6 ON

#### Thermistor Usage

In the GUI, click "Enable Thermistor" to start the example. Increase or decrease the
temperature and notice the RGB LED turn blue (colder), red (hotter) or green (same).
Click "Enable Thermistor" again to stop the example and return to the default
IDLE mode where LED1 toggles every 500ms.

##### Peripherals & Pin Assignments

- GPIOA
    + LED2.Red / PA26
	+ LED2.Blue / PA27
	+ LED2.Green / PA13
- TIMG0
- OPA0
    + OPA0_OUT / PA22
- ADC0
    + ADC0_9 / PA15

##### BoosterPacks, Board Resources & Jumper Settings
- Boosterpack connections:
    + J2 - PA0
- Board resources:
    + S2 - PA14 (GPIO_SWITCHES_USER_SWITCH_1_PIN)
    + LED2.Blue - PA27 (GPIO_RGB_BLUE_PIN)
    + LED2.Red - PA26 (GPIO_RGB_RED_PIN)
    + LED2.Green - PA13 (GPIO_RGB_GREEN_PIN)
- Jumper settings:
    + J1
        + [1:2] to enable thermistor to ADC
        + [2:3] to enable thermistor to OPA
    + J13 ON
    + J12 ON
    + J3 ON
