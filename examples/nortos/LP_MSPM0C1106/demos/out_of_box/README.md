# Out of Box Experience Summary

## Demo Summary

The out of box demo is a simple demo that implements various examples that showcase the peripherals functionality of the MSPM0C1106 LaunchPad. A GUI can be used with this demo, and the user can interact with the LaunchPad using the GUI. The GUI for this demo can be found [here](https://dev.ti.com/gallery/view/7482400/LP_MSPM0C1106_OOBE/ver/1.0.0/).

Please contact your local sales representative or TI support team to request access to the GUI.

### Demo Examples

#### Blinking LED Summary

Toggles LED1 at a frequency that is configurable by the accompanying GUI. LED1 will be off by default.

#### Button Counter Summary

Uses S2 to increment counter each time button is pressed. Counter will cycle from 0 to 10.

#### Temperature Sensor Summary

Display the collected ADC values of temperature sensor on the graph.

## Example Usage

* Open a serial session (e.g. [`PuTTY`](http://www.putty.org/ "PuTTY's
  Homepage"), etc.) to the appropriate COM port.
    * The COM port can be determined via Device Manager in Windows or via `ls /dev/tty*` in Linux.

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

* Indicator LED meaning:

  (*Required: The J9 Jumpers related to RGB LED_GREEN on the LaunchPad must be connected.*)

  - IDLE_MODE: default behavior, when no modes are active.
    - LED_GREEN will blink.

  - Enable a specific mode: Blink_LED / Button Counter / Temperature Sensor.
    - LED_GREEN will remain on.

#### Blinking LED Usage

In the GUI, click "Enable Blink LED" to start the example, the LED_GREEN will remain on. Drag the slider to adjust the LED toggle rate. Click "Enable Blink LED" again to stop the example and return to the default IDLE mode where LED_GREEN will remain blinking.

##### Peripherals & Pin Assignments

- TIMG8
- GPIOA
  + Output: PA0

##### BoosterPacks, Board Resources & Jumper Settings

- Board resources:
  + LED1 - PA0 (GPIO_LEDS_USER_LED_1_PIN)
- Jumper settings:
  + J4 ON - PA0

#### Button Counter Usage

In the GUI, click "Enable Button Counter" to start the example, the LED_GREEN will remain on. On the Launchpad, press S2 to see the counter within the red box increment. With each press, the counter will increment by 1 until it reaches 10, and then reset to 0. Click "Enable Button Counter" again to stop the example and return to the default IDLE mode where LED_GREEN will remain blinking.

##### Peripherals & Pin Assignments

- GPIOA
  - Input: PA14

##### BoosterPacks, Board Resources & Jumper Settings

- Board resources:
  - S2 - PA14 (SWITCHES_USER_SWITCH_1_PIN)

#### Temperature Sensor Usage

In the GUI, click "Enable Temperature Measurement" to start the example, the LED_GREEN will remain on. The ADC-collected data from the internal temperature sensor will be immediately displayed on the GUI graph. The actual temperature value obtained from the raw ADC data will also be displayed synchronously. Click "Enable Temperature Measurement" again to stop the example and return to the default IDLE mode where LED_GREEN will remain blinking.

##### Peripherals & Pin Assignments

- ADC12_0
  - Internal Temperature Sensor (DL_ADC12_INPUT_CHAN_28)

