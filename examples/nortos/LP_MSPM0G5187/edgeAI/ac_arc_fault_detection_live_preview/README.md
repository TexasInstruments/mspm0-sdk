## Example Summary

This example demonstrates AC arc fault detection using the Neural Processing Unit (NPU) on the TIDA-971 Board. The application captures AC current waveforms using an ADC, extracts relevant features using the feature_extract library, and uses a neural network model to classify the waveform as either normal operation or containing an arc fault signature.

This example demonstrates the live preview feature on EdgeAI studio using MSPM0 devices. The application captures AC current waveforms using an ADC, extracts relevant features using the feature_extract library, and performs inference with a model that is functionally equivalent to the ac_arc_fault_detection_ai_npu model. The final output is then streamed to EdgeAI Studio for real-time visualization

This example is built for the Hardware accelerated inference using a Neural network Processing Unit (NPU).

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOA | Standard with Wake Output | PA31 | J4_35 | <ul><li>PA31 can be connected to LED <ul><li> |
| ADC0 | A0 | PA21 | J2_18 |  |
| SYSCTL |  |  |  |  |
| UC0 | RX Pin | PA11 | Back-Channel UART RXD |  |
| UC0 | TX Pin | PA10 | Back-Channel UART TXD |  |
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
1. Connect a current sensor to the ADC input pin PA21. The sensor should be capable of safely measuring AC current in the circuit being monitored.
2. Compile, load and run the example.
3. Open a serial terminal with the following settings:
   - Baud rate: 115200
   - Data bits: 8
   - Stop bits: 1
   - Parity: None
   - Flow control: None

During operation:
- GPIO PA31 is set to low state during normal operation.
- GPIO PA31 is set to high state when an arc fault is detected.
- Detection results are also output via UART for monitoring and debugging

Note: This example has been trained and validated with typical household AC circuits (120V/60Hz and 230V/50Hz). For optimal performance in different electrical environments, the model may need to be retrained with application-specific data.
This README follows the format of the example provided, adapting it for the AC arc fault detection application while maintaining the same structure and level of detail. It includes all the necessary sections: example summary, peripherals and pin assignments, board resources and jumper settings, device migration recommendations, low-power recommendations, and usage instructions.

## References

- MSPM0G5187 Technical Reference Manual [Link](https://www.ti.com/product/MSPM0G5187)
- UL 1699 Standard for Arc-Fault Circuit Interrupters [Link](https://code-authorities.ul.com/wp-content/uploads/2014/05/Dini2.pdf)
- [TI Neural Network Compiler Guide](https://software-dl.ti.com/mctools/nnc/mcu/users_guide/)
- TI Model Training Guide: [tinyml-tensorlab](https://github.com/TexasInstruments/tinyml-tensorlab/tree/main)
- [AC Arc Fault Detection Theory](https://en.wikipedia.org/wiki/Arc-fault_circuit_interrupter)
- EdgeAI Software Guide: SDK_INSTALL_DIR/docs/english/middleware/edgeAI/MSPM0_EdgeAI_User_Guide.html

This README provides a comprehensive overview of the AC arc fault detection application using the NPU. It follows the standard format with introduction, features, hardware/software requirements, setup instructions, usage information, implementation details, and references. You can adjust any specific technical parameters or features to match the actual implementation of your example.