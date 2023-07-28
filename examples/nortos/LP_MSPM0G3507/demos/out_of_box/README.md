---
# out_of_box_mspm0g3507

---

## Demo Summary

The out of box demo is a simple demo that implements various examples that
showcase the analog functionality of the MSPM0G3507 LaunchPad.
A GUI can be used with this demo, and the user can interact with the
LaunchPad using the GUI. The GUI for this demo can be found
[here](https://dev.ti.com/gallery/view/TIMSPGC/LP_MSPM0G3507_OOBE/).

Please contact your local sales representative or TI support team to request access
to the GUI.

### Demo Examples

#### Blinking LED Summary

Toggle LED2, the RGB LED, at a frequency that is configurable by the
accompanying GUI. LED2 will be green and will toggle every 500ms by default.
#### Light Sensor Summary

Use OPA0 in general purpose mode along with the on-board photodiode,
capacitor, and resistor to implement a light sensor circuit.
The resulting voltage is measured by the ADC0 module.
The ADC result is used to control TimerG10's and TimerA2's PWMs
duty-cycles to modulate the brightness of the RGB LED. The ADC result will
also be displayed on the GUI.
#### Thermistor Summary

Use OPA0 in buffer mode along with the on-board thermistor to implement a temperature sensing circuit. The resulting voltage is measured by the internal ADC0 module. The ADC result is used to set the color of the RGB LED.
Temperature drops of over 2 degrees Celcius will result in a blue LED while temperature increases of over 2 degrees Celcius will result in a red LED. Temperature changes less than 2 degrees Celcius will result in a green LED.
The ADC result will also be displayed on the GUI.
#### Function Generator Summary

Generate a signal using the DAC. The DMA will load data into the DAC since
the DAC is operating at max sampling rate. The user can view the signal on
the OPA output pin, or they can have the signal be captured with the internal
ADC connection and displayed on the GUI.

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

* Run the demo. As mentioned above, the demo implements several examples. Use S3
on the LaunchPad or interact with the GUI to switch between examples. The specific
examples and their usage will be listed below.

#### Blinking LED Usage

In the GUI, click "Enable Blink LED" to start the example. Drag the slider to adjust the
LED toggle rate. Click "Enable Blink LED" again to stop the example and return
to the default IDLE mode where LED1 will toggle every 500ms.

##### Peripherals & Pin Assignments

- TIMA0
- GPIOB
    + Input with internal pull-up: PB21
    + Output: PB27

##### BoosterPacks, Board Resources & Jumper Settings

- Boosterpack connections:
    + J4 - PA0
- Board resources:
    + S2 - PB21 (GPIO_SWITCHES1_USER_SWITCH_1_PIN)
    + LED2.Green - PB27 (GPIO_RGB_GREEN_PIN)
- Jumper settings:
    + J7 ON
    + J19
        + [1:2] to enable pull-up in 5V mode or
        + [2:3] to enable pull-up in 3.3V mode

#### Light Sensor Usage

In the GUI, click "Enable Light Sensor" to start the example. Increase or decrease the
amount of light hitting the light sensor and notice the RGB LED brightening or dimming.
Click "Enable Light Sensor" again to stop the example and return to the default
IDLE mode where LED1 toggles every 500ms.

##### Peripherals & Pin Assignments

- GPIOB
    + Input with internal pull-up: PB21
    + Output: PB26
	+ Output: PB22
	+ Output: PB27
- TIMG0
- TIMG8
- TIMA1
- OPA0
    + OPA0_IN0+ / PA26
    + OPA0_IN0- / PA27
    + OPA0_OUT / PA22
- ADC0
    + ADC0_7 / PA22

##### BoosterPacks, Board Resources & Jumper Settings

- Boosterpack connections:
    + J4 - PA0
- Board resources:
    + S2 - PB21 (GPIO_SWITCHES1_USER_SWITCH_1_PIN)
    + LED2.Blue - PB22 (GPIO_RGB_BLUE_PIN)
    + LED2.Red - PB26 (GPIO_RGB_RED_PIN)
    + LED2.Green - PB27 (GPIO_RGB_GREEN_PIN)
- Jumper settings:
    + J5 ON
    + J6 ON
    + J7 ON
    + J16 ON
    + J17 ON
    + J18 ON
    + J19
        + [1:2] to enable pull-up in 5V mode or
        + [2:3] to enable pull-up in 3.3V mode

#### Thermistor Usage

Click "Enable Thermistor" to start the example. Increase or decrease the temperature and notice LED2 changing color based on the difference between the measured temperature and the threshold.
Click "Enable Thermistor" again to stop the example.

##### Peripherals & Pin Assignments

- GPIOB
    + LED2.Red / PB26
	+ LED2.Blue / PB22
	+ LED2.Green / PB27
- TIMG0
- OPA0
    + OPA0_OUT / PA22
- ADC0
    + ADC0_5 / PB24

##### BoosterPacks, Board Resources & Jumper Settings

- Boosterpack connections:
    + J4 - PA0
- Board resources:
    + S2 - PB21 (GPIO_SWITCHES1_USER_SWITCH_1_PIN)
    + LED2.Blue - PB22 (GPIO_RGB_BLUE_PIN)
    + LED2.Red - PB26 (GPIO_RGB_RED_PIN)
    + LED2.Green - PB27 (GPIO_RGB_GREEN_PIN)
- Jumper settings:
    + J9
        + [1:2] to enable thermistor signal to ADC directly
        + [2:3] to enable GPAMP module for thermistor
    + J19
        + [1:2] to enable pull-up in 5V mode or
        + [2:3] to enable pull-up in 3.3V mode

#### Function Generator Usage

Click "Enable Function Generator" to start the example. Generate a signal using
the DAC by selecting the signal and op-amp parameters. If "Plot ADC" is set,
the signal will be fed to the ADC and plotted on the GUI. If "Plot ADC" is not set,
the signal will be fed to the op-amp output pin and can be captured via scope.
Click "Enable Function Generator" again to stop the example.

##### Peripherals & Pin Assignments

- DMA
- DAC
- TIMA0
- OPA1
    + OPA1-OUT / PA16
- ADC1
    + ADC1_13 (internal connection)
- DAC
    + DAC-OUT / PA15

##### BoosterPacks, Board Resources & Jumper Settings

- Jumper settings:
    + J19
        + [1:2] to enable pull-up in 5V mode or
        + [2:3] to enable pull-up in 3.3V mode
