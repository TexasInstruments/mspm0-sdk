## Example Summary

This code example configures the USB peripheral in device mode with the
Communication Device Class and its subclass Abstract Control Model (CDC->ACM).
When the USB peripheral is configured in such a manner it behaves as a serial port
that users can send and receive data through.

## Example Usage

To utilize this example please connect USB2 on the Launchpad to a host that contains a
serial terminal. Two serial ports will be enabled and when configuring each serial
port in the settings of the serial terminal set them to the values provided below
in the Serial Settings table. Each serial port allows the user to input data that
will then be echoed back to the user in the terminals. Both ports will receive the
same data no matter which port is being written to, only when the data is echoed
back, differences between the two port can be seen. One port will echo lower-case
characters and the other port will echo upper-case characters.

## Serial Settings

| Serial Setting | Value |
| --- | --- |
| Baud Rate | 115200 |
| Data Bits | 8 |
| Stop Bits | 1 |
| Parity | None |
| Flow Control | None |
