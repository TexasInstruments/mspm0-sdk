## Example Summary

This example implements a data-capture for motor fault demo for the MSPM0G5187.  
The device acquires vibration samples from an ADXL355 accelerometer and streams raw time-domain samples to a host over the Device Agent Protocol (DAP) via UART/USB. The host tool records and visualizes the acquired samples for offline model training, verification, or analysis.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOA | High-Speed Input | PA12 | J3_28 |  |
| SYSCTL |  |  |  |  |
| UC0 | RX Pin | PA11 | Back-Channel UART RXD |  |
| UC0 | TX Pin | PA10 | Back-Channel UART TXD |  |
| UC2 | SPI SCLK (Clock) | PB18 | J1_7 |  |
| UC2 | SPI PICO (Peripheral In, Controller Out) | PB17 | J2_15 |  |
| UC2 | SPI POCI (Peripheral Out, Controller In) | PB19 | J2_14 |  |
| UC2 | SPI CS1 (Chip Select 1) | PB0 | J4_38 |  |
| EVENT |  |  |  |  |
| DMA |  |  |  |  |
| BOARD | Debug Clock | PA20 | J101_16 |  |
| BOARD | Debug Data In Out | PA19 | J101_14 |  |

### Low-Power Recommendations

Terminate unused pins as GPIO outputs driven low or configure as inputs with pull-ups/pull-downs. Use SysConfig "Board → Configure Unused Pins" to apply recommended settings. Consult the LP-MSPM0G5187 user guide for jumper settings to reduce power.

## Hardware requirements

1. MSPM0G5187 development board / LaunchPad
2. ADXL355 accelerometer connected over SPI
3. Host PC with a USB/serial connection to the board (or USB DFU supporting the live DAP bridge)

## Example Usage

1. **Hardware Setup**
   - Connect ADXL355 SPI lines to SPI0 (PB18 SCLK, PB17 MOSI, PB19 MISO, PB0 CS).
   - Connect ADXL355 DRDY (data-ready) to PA12 (GPIO interrupt).
   - Power the sensor from the board's regulated 3.3V and ensure common ground.
   - Secure the accelerometer to the device under test (motor) for accurate vibration coupling.

2. **Operation**
   - On startup, firmware initializes SPI, GPIO, interrupts and UART/DAP components and configures the ADXL355 (range, power control, interrupt map).
   - The device waits for host commands over DAP. The host can request a capture by issuing the start-streaming command.
   - When streaming is active, each DRDY interrupt triggers SPI reads to collect XYZ samples. Samples are queued and transmitted to the host using the DAP framing over UART.
   - The DAP framing supports variable payload lengths; the firmware encodes frame start, channel, length and payload, and ends with a frame-end marker.
   - The host receives raw time-domain samples (interleaved X,Y,Z) and saves it in the csv format.

3. **Running the Example (Host)**
   - Build and flash the firmware to the MSPM0G5187.
   - On the host PC run the provided capture/listen tool or Model Composer Edge AI Studio Live Preview set to "Data Capture" mode.
   - From Model Composer / Edge AI Studio connect to the device and request a data capture session via the GUI.
   - Make sure that the correct baudrate (115200) and COM ports are selected before connecting to the device.

## Software Details

- Data acquisition reads ADXL355 XYZ samples on each DRDY event using SPI.
- Samples are packed as 16-bit signed integers (high byte first) and streamed in DAP frames.
- UART settings: 115200 baud.
- Key configurable property: "time" — the host sets how many samples to stream/transfer based on sampling rate (1024 deafult)*time.

## Notes

- Ensure the ADXL355 sensor is properly powered and connected.
- For more information on the ADXL355, refer to the datasheets and user guides.
   - [Datasheet](https://www.analog.com/media/en/technical-documentation/data-sheets/adxl354_adxl355.pdf)
   - [User guide](https://www.digikey.in/en/htmldatasheets/production/2012480/0/0/1/eval-adxl354-355-user-guide)
- ADXL355 manufacturer page [link](https://www.analog.com/en/index.html)