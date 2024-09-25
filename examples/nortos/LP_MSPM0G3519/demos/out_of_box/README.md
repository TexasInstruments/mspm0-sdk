---
# out_of_box_mspm0g3519

---

## Demo Summary

The out of box demo is a simple demo that implements various examples that
showcase the functionality of the MSPM0G3519 LaunchPad.
A GUI can be used with this demo, and the user can interact with the
LaunchPad using the GUI. The GUI for this demo can be found
[here](https://dev.ti.com/gallery/view/TIMSPGC/LP_MSPM0G3519_OOBE/).

### Demo Examples

#### Blinking LED Summary

Toggle LED2, the RGB LED, at a frequency that is configurable by the
accompanying GUI. LED2 will be green and will toggle every 500ms by default.

#### Temperature Summary

Using the internal temperature sensor and ADC, the ambient temperature will
be measured and displayed by the GUI. When S1 is pressed,
the units for the temperature will change from Celsius to Fahrenheit
and vice versa.

#### Function Generator Summary

Generate a signal using the DAC. The DMA is used to load the sample values to the DAC. 
The user can view the signal on the DAC output pin (PA15), or they can view the output
in the GUI, after it is captured by the ADC internally. 

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

## Peripherals & Pin Assignments

- TIMA0
- GPIOB
    + Input with internal pull-up: PB3
    + Output: PB27

## BoosterPacks, Board Resources & Jumper Settings

- Boosterpack connections:
- Board resources:
    + S2 - PB3 (GPIO_SWITCHES_USER_SWITCH_2_PIN)
    + LED2.Green - PB27 (GPIO_RGB_GREEN_PIN)
- Jumper settings:
    + J9 ON

#### Temperature Usage

Click "Enable Temperature Mode" to start the example.   
Click "Enable Temperature Mode" again to stop the example.
Switch 1 (S1) or the GUI can be used to change the units between Celcius and Fahrenheit

## Peripherals & Pin Assignments

- GPIOB
    + LED2.Red / PB26
	+ LED2.Blue / PB22
	+ LED2.Green / PB27
- TIMG0
- ADC0
    + ADC0_11

## BoosterPacks, Board Resources & Jumper Settings

- Board resources:
    + S1 - PA18 (GPIO_SWITCHES1_USER_SWITCH_1_PIN)
    + LED2.Blue - PB22 (GPIO_RGB_BLUE_PIN)
    + LED2.Red - PB26 (GPIO_RGB_RED_PIN)
    + LED2.Green - PB27 (GPIO_RGB_GREEN_PIN)
- Jumper settings:
    + J7
        + [1:2] to enable RGB Blue LED
    + J8
        + [1:2] to enable RGB Red LED
    + J9
        + [1:2] to enable RGB Green LED

#### Function Generator Usage

Click "Enable Function Generator" to start the example. Generate a signal using  
the DAC by selecting the signal and op-amp parameters. If "Plot ADC" is set,  
the signal will be fed to the ADC and plotted on the GUI. If "Plot ADC" is not set,  
the signal can be captured via scope on the DAC output pin.  
Click "Enable Function Generator" again to stop the example.

## Peripherals & Pin Assignments

- DMA
- DAC0
- TIMA0
- ADC1
    + ADC1_0 (internal connection) 
- DAC0
    + DAC-OUT / PA15

## BoosterPacks, Board Resources & Jumper Settings

- Jumper settings:
    + J2
        + [1:2] to enable pull-up in 5V mode or
        + [2:3] to enable pull-up in 3.3V mode