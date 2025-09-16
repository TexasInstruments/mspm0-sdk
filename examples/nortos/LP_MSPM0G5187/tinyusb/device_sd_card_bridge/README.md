## Example Summary

This example configures the USB peripheral in device mode with Mass Storage Class (MSC) support.
It creates a bridge between the USB host and an SD card connected to the Launchpad, allowing the
host to directly access and modify files on the SD card.

## Example Usage

Insert an SD card into the slot on the Launchpad, then connect the USB2 port on the Launchpad
to any host computer. Once connected, the SD card will appear as a removable storage device
on the host. Use your computer's file explorer to browse, read, and write files directly to
the SD card. Any changes made on the host will be written to the physical SD card
