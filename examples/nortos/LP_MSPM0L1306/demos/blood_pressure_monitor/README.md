## Example Summary

This code example demonstrates how to implement a blood pressure monitor demo.

A GUI can be used with this demo, and the user can interact with the code using the GUI. The GUI for this demo and its instructions can be found
[here](https://dev.ti.com/gallery/view/TIMSPGC/BLOOD_PRESSURE_MONITOR/).

A reference design, [TIDA-010266](https://www.ti.com/tool/TIDA-010266), will be released in 2023 as a guide on how to design a blood pressure monitor system using the MSPM0L. It will include suggestions on how to design a custom board and code for this system and further explain the provided implementation.

## Peripherals & Pin Assignments
- ADC0
    + ADC0_0 / PA27
    + ADC0_1 / PA26
- TIMG1
- TIMG2
- Two op-amp INA
    + OPA0
        + OPA0_IN0- / PA24
        + OPA0_IN0+ / PA25
        + OPA0_OUT / PA22
    + OPA1
        + OPA1_IN0- / PA17
        + OPA1_IN0+ / PA18
        + OPA_OUT / PA16

OPA0 and OPA1 are used together as an instrumentation amplifier.
