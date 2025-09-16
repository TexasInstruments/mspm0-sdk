## Example Summary

This code example configures the USB peripheral in host mode with the
Mass Storage Class (MSC). Upon connecting a USB device to the host, it
will enumerate and provide a file system to traverse through the content.

## Example Usage

To utilize this example move jumpers J25/J26 to the HOST side.
Once the jumpers have been correctly configured open a serial port
to interface with the UART terminal using the settings provided below.
Connect a mass storage class device to USB2 on the Launchpad, upon
succesful enumeration with the USB device a file system will be available
to traverse through the content of the device via the terminal.

## Serial Settings

| Serial Setting | Value |
| --- | --- |
| Baud Rate | 115200 |
| Data Bits | 8 |
| Stop Bits | 1 |
| Parity | None |
| Flow Control | None |
