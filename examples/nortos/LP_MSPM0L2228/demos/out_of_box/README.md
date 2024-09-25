---
# out_of_box_mspm0l2228

---

## Demo Summary

The out of box demo is a simple demo that implements various examples that
showcase the functionality of the MSPM0L2228 LaunchPad.
A GUI can be used with this demo, and the user can interact with the
LaunchPad using the GUI. The GUI for this demo can be found
[here](https://dev.ti.com/gallery/view/TIMSPGC/LP_MSPM0L2228_OOBE/).

### Demo Examples

#### Blinking LED Summary

Toggle LED2, the RGB LED, at a frequency that is configurable by the
accompanying GUI. LED2 will be green and will toggle every 500ms by default.

#### Temperature Summary

Using the internal temperature sensor and ADC, the ambient temperature will
be measured and displayed on the LCD screen. When S3 is pressed,
the units for the temperature will change from Celsius to Fahrenheit
and vice versa.

#### Tamper Event Summary
An RTC time stamp will be taken when a tamper event is detected.
The tamper event is caused by a rising edge on the tamperIO pin, PA7.
The RTC value will be displayed on the LCD screen.

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
- GPIOA
	+ Switch 2 - PA18
- GPIOB
    + Output: PB9

##### BoosterPacks, Board Resources & Jumper Settings

- Boosterpack connections:
- Board resources:
    + S1 - PA18 (GPIO_SWITCHES1_USER_SWITCH_1_PIN)
    + LED2.Green - PB9 (GPIO_RGB_GREEN_PIN)
- Jumper settings:
    + J7 ON

#### Temperature Usage

Click "Enable Temperature" to start the example.
Increase or decrease the temperature and notice value on the LCD screen change.
Click "Enable Temperature" again to stop the example.

##### Peripherals & Pin Assignments

- GPIOA
    + Switch 1/ PA18
- TIMG0
- ADC0
    + ADC0_29 / Temperature Sensor
- LCD

##### BoosterPacks, Board Resources & Jumper Settings

- Boosterpack connections:
- Board resources:
    + S1 - PA18
   	+ LCD

#### Tamper Event Usage

An RTC time stamp will be taken when a tamper event is detected. The tamper
event is caused by a rising edge on the tamperIO pin, PA7. The RTC value
will be displayed on the LCD screen.

##### Peripherals & Pin Assignments

- GPIOA
	+ TamperIO_0 / PA7
- LCD
- RTC

##### BoosterPacks, Board Resources & Jumper Settings

- Boosterpack connections:
- Board resources:
	+LCD
- Jumper settings:
