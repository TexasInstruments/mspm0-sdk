## Example Summary

This code example configures the USB peripheral in host mode without
any specific class. Upon connecting a USB device to the host, it
will enumerate and print out the descriptor via serial terminal of choice.

## Example Usage

To utilize this example move jumpers J25/J26 to the HOST side.
Once the jumpers have been correctly configured open a serial port
to interface with the UART terminal using the settings provided below.
Connect a USB device to USB2 on the Launchpad, upon succesful enumeration
with the USB device the device descriptor will be printed to the serial
terminal.

## Serial Settings

| Serial Setting | Value |
| --- | --- |
| Baud Rate | 115200 |
| Data Bits | 8 |
| Stop Bits | 1 |
| Parity | None |
| Flow Control | None |
