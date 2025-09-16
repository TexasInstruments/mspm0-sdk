## Example Summary

This code example configures the USB peripheral in device mode as a Billboard device. The Billboard Device Class is used to provide alternate mode capability information when the device is unable to function in the requested USB Type-C alternate mode.

## Example Usage

When S2 on the Launchpad is pressed, the Billboard descriptor is updated to cycle through the supported alternate modes. The device will disconnect and reconnect to the host to apply the new descriptor information.

1. USB4 mode
2. DisplayPort mode
3. Thunderbolt mode

This example demonstrates how to implement a dynamic Billboard device that can update its capabilities and inform USB Type-C hosts about supported alternate modes.
