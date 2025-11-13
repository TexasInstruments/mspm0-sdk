## Example Summary

This example demonstrates the live preview feature on EdgeAI studio using MSPM0 devices. The application captures an input waveform via the ADC, extracts relevant features using the feature_extract library, and performs inference with a model that is functionally equivalent to the waveform_classifier_ai_npu model. The final output is then streamed to EdgeAI Studio for real-time visualization

This example is built for the Hardware accelerated inference using a Neural network Processing Unit (NPU).
 
## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOA | Standard with Wake Output | PA17 | LED2 Red | Populate Jumper(s): J12[2:1] |
| GPIOA | Standard Output | PA24 | LED2 Green | Populate Jumper(s): J13[2:1] |
| GPIOB | Standard Output | PB13 | LED2 Blue | Populate Jumper(s): J11[2:1] |
| VREF | VREF Positive (VREF+) | PA23 | | |
| VREF | VREF Negative (VREF-) | PA21 | | |
| SYSCTL |  |  |  |  |
| UC0 | RX Pin | PA11 | Back-Channel UART RXD |  |
| UC0 | TX Pin | PA10 | Back-Channel UART TXD |  |
| ADC0 | ADC12 Channel 5 Pin | PB24 | J2_18 |  |
| EVENT |  |  |  |  |
| DMA |  |  |  |  |
| BOARD | Debug Clock | PA20 | J101_16 |  |
| BOARD | Debug Data In Out | PA19 | J101_14 |  |

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
MSPM0 LaunchPad, please visit the LP-MSPM0G5187 User's Guide.

## Example Usage

To initiate the live preview:
1. Configure the serial port in EdgeAI Studio. Confirm that the port and baud rate (115200 bps) are selected to match the device's UART baudrate.
2. Before starting, check the status bar in EdgeAI studio for the message "**Hardware connected**"
3. Navigate to the **Live preview** tab
4. Click **Start Preview** to begin viewing the real-time model output.

## References

- MSPM0G5187 Technical Reference Manual [Link](https://www.ti.com/product/MSPM0G5187)
- [TI Neural Network Compiler Guide](https://software-dl.ti.com/mctools/nnc/mcu/users_guide/)
- TI Model Training Guide: [tinyml-tensorlab](https://github.com/TexasInstruments/tinyml-tensorlab/tree/main)
- EdgeAI Software Guide: SDK_INSTALL_DIR/docs/english/middleware/edgeAI/MSPM0_EdgeAI_User_Guide.html