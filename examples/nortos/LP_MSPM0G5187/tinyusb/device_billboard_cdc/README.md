## Example Summary

This code example configures the USB peripheral in device mode as a composite Billboard and CDC (Communication Device Class) device. It demonstrates how to implement a USB Type-C Billboard device that also provides CDC-ACM serial port functionality. The Billboard Class provides alternate mode capability information when the device is unable to function in the requested USB Type-C alternate mode.

## Example Usage

To utilize this example, connect USB2 on the Launchpad to a USB Type-C host. The device will enumerate as both a Billboard device and a CDC device (virtual serial port).

The Billboard component provides information about supported alternate modes that can be cycled through by pressing the S2 on the Launchpad:

1. DisplayPort mode
2. Thunderbolt mode
3. USB4 mode

When S2 is pressed, the Billboard descriptor is updated, and the device will disconnect and reconnect to the host to apply the new descriptor information.

At the same time, the CDC interface provides a simple echo function. Connect to the virtual serial port using a terminal program configured with the settings below. Any data received through the CDC serial port will be echoed back to the sender.

## Serial Settings

| Serial Setting | Value |
| --- | --- |
| Baud Rate | 115200 |
| Data Bits | 8 |
| Stop Bits | 1 |
| Parity | None |
| Flow Control | None |
