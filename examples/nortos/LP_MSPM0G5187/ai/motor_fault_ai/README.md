## Example Summary

This example demonstrates a motor fault detection application using an AI model on MSPM0G5187 microcontroller. The system collects vibration data from an ADXL355 accelerometer, extracts frequency-domain features using FFT, and performs inference using a pre-trained neural network model. The detected fault class is indicated by toggling onboard LEDs.

In this example the inference happens using the M0+ CPU itself and not hardware accelerator is used.

The class to LED color mapping is shown below:
   - **Blue**: Class 0 (Normal)
   - **Red**: Class 1 (Blade Damage)
   - **Green**: Class 2 (Blade Imbalance)
   - **White**: Class 3 (others or unknown)

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOA | Standard with Wake Output | PA17 | LED2 Red | Populate Jumper(s): J12[2:1] |
| GPIOA | Standard Output | PA24 | LED2 Green | Populate Jumper(s): J13[2:1] |
| GPIOB | Standard Output | PB13 | LED2 Blue | Populate Jumper(s): J11[2:1] |
| GPIOA | Standard Input | PA12 | J3_28 |  |
| SYSCTL |  |  |  |  |
| SPI0 | SPI SCLK (Clock) | PB18 | J1_7 |  |
| SPI0 | SPI PICO (Peripheral In, Controller Out) | PB17 | J2_15 |  |
| SPI0 | SPI POCI (Peripheral Out, Controller In) | PB19 | J2_14 |  |
| SPI0 | SPI CS1 (Chip Select 1) | PB0 | J4_38 |  |
| EVENT |  |  |  |  |
| BOARD | Debug Clock | PA20 | J101_16 |  |
| BOARD | Debug Data In Out | PA19 | J101_14 |  |

### Low-Power Recommendations

Terminate unused pins by configuring them as GPIO outputs low or inputs with 
internal pull-up/pull-down resistors. Use SysConfig to easily configure unused pins.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0G5187 User's Guide](https://www.ti.com/lit/slau967).

## Hardware requirements

1. MSPM0G5187 microcontroller
2. ADXL355 accelerometer (sensor) [Link](https://www.digikey.in/en/products/detail/analog-devices-inc/EVAL-ADXL355-PMDZ/7324256?s=N4IgTCBcDaIKIDUCCAZAtEgIgDRQZgFYC0AFAWUwC0QBdAXyA)
3. EF80251S1-1000U-A99 (DC Brushless Fans - BLDC) [Link](https://www.digikey.in/en/products/detail/sunon-fans/EF80251S1-1000U-A99/6198727)

## Example Usage

1. **Hardware Setup**
   - Connect the ADXL355 accelerometer (SPI Peripheral) to the MSPM0 SPI interface (SPI Controller).
     - Controller SCLK -> Peripheral SCLK
     - Controller PICO -> Peripheral PICO
     - Controller POCI <- Peripheral POCI
     - Controller CS   -> Peripheral CS
   - Connect the sensor ready pin (Data Ready - DRDY) to the designated GPIO interrupt pin (PA12).
     - DRDY -> Standard Input
   - Ensure LEDs are connected to the appropriate GPIO pins for class indication.
   - Ensure that the digital ground and power from the microntroller is connected to respective pins on the sensor.
   - Stick the sensor to the top of the motor.

2. **Operation**
   - On startup, the application initializes the sensor and peripherals.
   - The ADXL355 accelerometer collects vibration data from the motor.
   - When new data is ready, an interrupt triggers data acquisition.
   - The application performs feature extraction using FFT and bins the frequency magnitudes.
   - Extracted features are fed to the AI model for inference.
   - The output class (fault type) is determined and stored in a circular buffer.
   - The LED corresponding to the most frequently detected class in the buffer is toggled:
     - **LED1 (Blue)**: Class 0 (Normal)
     - **LED2 (Red)**: Class 1 (Blade Damage)
     - **LED3 (Green)**: Class 2 (Blade Imbalance)
     - **All LEDs (White)**: Class 3 (others or unknown)

3. **Running the Example**
   - Compile, load, and run the application on your MSPM0 device.
   - The system will continuously monitor motor vibration and indicate detected fault classes via LEDs.

## Software Details

- **Feature Extraction**: Uses ARM CMSIS-DSP library for real FFT and magnitude calculation.
- **AI Inference**: Runs a pre-trained model using TVM runtime.
- **Interrupt Handling**: Efficient data collection via GPIO interrupts.
- **LED Indication**: Fault class is indicated by toggling onboard LEDs.
- **AI model used**: Model details find in tensor lab repo.

## Notes

- Ensure the ADXL355 sensor is properly powered and connected.
- Adjust LED and GPIO pin assignments as needed for your hardware.
- For more information on the ADXL355, refer to the datasheets and user guides.
  - [Datasheet](https://www.analog.com/media/en/technical-documentation/data-sheets/adxl354_adxl355.pdf)
  - [User guide](https://www.digikey.in/en/htmldatasheets/production/2012480/0/0/1/eval-adxl354-355-user-guide)
- ADXL355 manufacturer page [link](https://www.analog.com/en/index.html)