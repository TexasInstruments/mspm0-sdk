## Example Summary

This code example demonstrates how to implement a pulse oximeter demo.

A GUI can be used with this demo, and the user can interact with the code using the GUI. The GUI for this demo can be found
[here](https://dev.ti.com/gallery/view/TIMSPGC/PULSE_OXIMETER/).

A reference design, [TIDA-010267](https://www.ti.com/tool/TIDA-010267), will be released in 2023 as a guide on how to design a pulse oximeter system using the MSPM0L and will include suggestions for both board and code design.

## Peripherals & Pin Assignments
- TIMG0
- TIMG1
- OPA0
    + OPA0_IN0- / PA24
    + OPA0_IN0+ / PA25
    + OPA0_OUT / ADC0 / PA22
- OPA1
    + OPA1_OUT / PA16
- VREF
    + VREF+ / PA23
    + VREF- / PA21
