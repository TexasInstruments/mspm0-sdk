## Out of Box Experience Summary

The out of box demo is a simple demo that implements various examples that
showcase the analog functionality of the MSPM0C1104 LaunchPad.
A GUI can be used with this demo, and the user can interact with the
LaunchPad using the GUI. The GUI for this demo can be found
[here](https://dev.ti.com/gallery/view/TIMSPGC/LP_MSPM0C1104_OOBE/).

Please contact your local sales representative or TI support team to request access
to the GUI.

### Demo Examples

#### Blinking LED Summary

Toggles LED1 at a frequency that is configurable by the
accompanying GUI. LED1 will be on by default.

#### Button Counter Summary

Uses S2 to increment counter each time button is pressed. Counter will cycle
from 0 to 10.

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

* Run the demo. As mentioned above, the demo implements several examples.
The specific example and their usage will be listed below.

#### Blinking LED Usage

In the GUI, click "Enable Blink LED" to start the example. Drag the slider to adjust the
LED toggle rate. Click "Enable Blink LED" again to stop the example and return
to the default IDLE mode where LED1 will remain on.

##### Peripherals & Pin Assignments

- TIMG8
- GPIOA
    + Output: PA4

##### BoosterPacks, Board Resources & Jumper Settings

- Board resources:
    + LED1 - PA4 (GPIO_LEDS_USER_LED_1_PIN)
- Jumper settings:
    + J7 ON

#### Button Counter Usage

On the Launchpad, press S2 to see the counter within the red box increment.
With each press, the counter will increment by 1 until it reaches 10, and then
reset to 0.

##### Peripherals & Pin Assignments

- GPIOA
	+ Output: PA16

##### BoosterPacks, Board Resources & Jumper Settings

- Board resources:
    + S2 - PA16 (GPIO_SWITCHES_USER_SWITCH_1_PIN)
