## Example Summary

This empty example is a template for creating an application into which a trained AI/ML model may deployed.
This example expects a model which is compiled for execution on TI's neural processing unit (NPU) to be added
to the project after the project is imported.
 
## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| SYSCTL |  |  |  |  |
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
Before building the example, train and compile a machine learning model targeting
deployment to TI's neural processing unit (NPU).
See the [Deploying Machine Learning Models on TI MSPM0 Microcontrollers](https://www.ti.com/lit/sdaa039) documentation for
details on the process for training, compiling, and deploying machine learning models.
Once a compiled model is available for execution, create a folder called "model" in the project within
Code Composer Studio.  Then, insert the model.a and tvmgen_default.h files into this folder.
Create the input and output feature maps for passing data to the model and receiving the inference result from the model,
and add these symbols to the call to tvmgen_default_run in the while(1) loop in the empty_npu_ai.c file.
Once this is done, change the MODEL_AVAILABLE definition at the top of the empty_npu_ai.c file from '0' to '1'.  This
will include the TVM header and enable the call to tvmgen_default_run to run the inference.

Compile, load and run the example.

## References

- MSPM0G5187 Technical Reference Manual [Link](https://www.ti.com/product/MSPM0G5187)
- [TI Neural Network Compiler Guide](https://software-dl.ti.com/mctools/nnc/mcu/users_guide/)
- TI Model Training Guide: [tinyml-tensorlab](https://github.com/TexasInstruments/tinyml-tensorlab/tree/main)
- EdgeAI Software Guide: SDK_INSTALL_DIR/docs/english/middleware/edgeAI/MSPM0_EdgeAI_User_Guide.html