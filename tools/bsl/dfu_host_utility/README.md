# MSPM0 DFU Host Utility

This is a user guide on how to use MSPM0 DFU Host Utility to program, erase, unlock, and read memory from the MSPM0 device using a simple menu or command-line interface.

---

## Features

- Program device with `.txt` or `.bin` firmware
- Readback device memory to file in both `.txt` or `.bin`
- Standalone verification
- Mass erase and flash range erase
- Unlock BSL (requires password file)
- Factory reset (supports password file **or** no password)
- Get device info, start application
- Interactive menu or command-line flags
- Optional logging and "wrap only" (no send) mode

---

## Devices supported

- MSPM0G511x
- MSPM0G518x

---

## Installation

### 1. Setup dfu-util

- Download [dfu-util binaries](https://dfu-util.sourceforge.net/releases/) (version 0.7).
- Or search for [dfu-util binaries on Bing](https://www.bing.com/search?q=dfu-util+binaries).
- Place the downloaded `dfu-util-version-` folder or the folder containing the dfu-util.exe in your source directory.
- Change the folder name to `dfu-util` and place it into the `dfu_host_utility` folder.

### 2. Install Zadig USB Driver

- Download Zadig from [https://zadig.akeo.ie/](https://zadig.akeo.ie/) (version 2.0.1.160).
- Connect the device to your PC and ensure it's in DFU mode.
- Run `zadig.exe`.
- From the dropdown list, select the device named **Device Firmware Update** (USBID: VID 0x2047, PID 0x0210).
- If you don't see it, go to **Options > List All Devices**.
- In the driver selection box, choose **WinUSB**.
- Click **Install Driver** (or **Replace Driver** if already installed).

**Folder structure:**
```
dfu_host_utility
├─ dfu-util
│    ├─ dfu-util.exe 
│    ├─ libusb-1.0.dll
├─ inputs
├─ debug
├─ output
├─ hostutility_interface.exe
├─ hostutility.exe
├─ example.bat
├─ source_code
│    ├─ commands.h
│    ├─ hostutility_interface.c
│    ├─ hostutility.c 
│    ├─ dfu_host_utility_exe_update.bat
└─ README.md
```

**Important note:**
- The folder and file names are case-sensitive on some systems.
- Do not rename `dfu-util.exe`.
- Ensure all files and folders are in the correct locations before running the utility.
- Create the debug folder as given in the folder structure if not already present.

---

## Requirements

- Windows PC
- dfu-util and zadiag drivers
- MSPM0 device connected via USB to the PC (only one MSPM0 device has to be connected at a time)

---

## Getting Started

### 1. Connect Your Device

- Plug the MSPM0 device into the windows PC.
- To put the device into DFU mode follow the below steps:
  1. Hold the BSL invoke pin (see the hardware manual).
  2. Press and release the reset (NRST) button.
  3. Release the BSL invoke pin.
  4. PC should show "DEVICE FIRMWARE UPGRADE" in Device Manager.

### 2. Using the executables

#### a. Interactive mode

- Double-click `hostutility_interface.exe` or run it from the command prompt.
- It provides an interface to program or verify the device memory through the DFU interface.
- The utility will guide you step-by-step and ask for any required files or information.
- In case of upload or download error and logs disable, you can check the logs in the debug folder.

#### b. Script mode

**Command-Line Flags:**

  | Flag | Description                                                               |
  |------|---------------------------------------------------------------------------|
  | `-c` | Command to execute (see below)                                            |
  | `-i` | Input file (`.txt` for TI-TXT, `.bin` for binary, or password for unlock) |
  | `-a` | Start address (required for `.bin` programming, readback, etc.)           |
  | `-e` | End address (for range erase)                                             |
  | `-l` | Length (for readback, verification)                                       |
  | `-o` | Output file name (for response/readback or wrapped image)                 |
  | `-v` | Enable verbose logging of dfu-util                                        |
  | `-s` | Only wrap file, do not send to device                                     |
  | `-h` | Show help message                                                         |
  | `-d` | Sets Device Id                                                            |
  | `-v` | Sets Vendor ID                                                            |

**Supported Commands:**

  | Command                      | Description                                                        |
  |----------------------------- |--------------------------------------------------------------------|
  | `CMD_PROGRAM_DATA`           | Programs    (needs image file in txt or bin)                       |
  | `CMD_CONNECTION`             | Set interface as DFU                                               |
  | `CMD_UNLOCK_BSL`             | Unlock BSL (needs password `.txt` file)                            |
  | `CMD_GET_IDENTITY`           | Get device information (requires output file)                      |
  | `CMD_MASS_ERASE`             | Mass erase device                                                  |
  | `CMD_FACTORY_RESET`          | Factory reset (supports password `.txt` file **or** no password)   |
  | `CMD_STANDALONE_VERIFICATION`| Standalone verification (requires output file, address and length) |
  | `CMD_START_APPLICATION`      | Start application on device                                        |
  | `CMD_FLASH_RANGE_ERASE`      | Erase flash range (needs start,end address)                        |
  | `CMD_MEMORY_READ_BACK`       | Read memory from device (requires output file, address and length) |

**Usage template:**

- Open a command prompt in the `dfu_host_utility`.

Format for the commands are as follows:

- Program with a `.txt` file:
  ```sh
  hostutility.exe -c CMD_PROGRAM_DATA -i <file_name>.txt
  ```
- Program with a `.bin` file (address required):
  ```sh
  hostutility.exe -c CMD_PROGRAM_DATA -i <file_name>.bin -a <start_address> -o <output_file_name>.dfu
  ```
- Read memory into `.bin` file (address and length required):
  ```sh
  hostutility.exe -c CMD_MEMORY_READ_BACK -a <start_address> -l <length> -o <file_name>.bin
  ```
- Read memory into `.txt` file (address and length required):
  ```sh
  hostutility.exe -c CMD_MEMORY_READ_BACK -a <start_address> -l <length> -o <file_name>.txt
  ```
- Unlock BSL (password file required):
  ```sh
  hostutility.exe -c CMD_UNLOCK_BSL -i <password_file>.txt
  ```
- Factory Reset (with password file):
  ```sh
  hostutility.exe -c CMD_FACTORY_RESET -i <password_file>.txt
  ```
- Factory Reset (without password):
  ```sh
  hostutility.exe -c CMD_FACTORY_RESET
  ```
- Connection:
  ```sh
  hostutility.exe -c CMD_CONNECTION
  ```
- Mass erase:
  ```sh
  hostutility.exe -c CMD_MASS_ERASE
  ```
- Start application:
  ```sh
  hostutility.exe -c CMD_START_APPLICATION
  ```
- Standalone verification with `.bin` as output file format
  ```sh
  hostutility.exe -c CMD_STANDALONE_VERIFICATION -a <start_address> -l <length> -o <file_name>.bin
  ```
- Standalone verification with `.txt` as output file format
  ```sh
  hostutility.exe -c CMD_STANDALONE_VERIFICATION -a <start_address> -l <length> -o <file_name>.txt
  ```
- Flash range erase
  ```sh
  hostutility.exe -c CMD_FLASH_RANGE_ERASE -a <start_address> -e <end_address>
  ```
- Get identity
  ```sh
  hostutility.exe -c CMD_GET_IDENTITY -o <file_name>.txt
  ```
- For more options, run:
  ```sh
  hostutility.exe -h
  ```

---

## Licensing Note for Third-Party Tools

The licenses for the third-party open source tools are as follows:

- **Zadig**  
  Zadig (version number - 2.0.1.160) - USB driver installation made easy - GNU General Public License (GPL) version 3 or later.  
  You are free to distribute, modify or even sell the software, insofar as you respect the GPLv3 license.  
  Zadig is based on libwdi which uses an LGPL version 3 or later license.  
  [Zadiag License Information](https://zadig.akeo.ie/)

- **dfu-util**  
  dfu-util (version number - 0.7) is licensed under the GNU General Public License (GPL) Version 2.  
  [dfu-util License Information](https://dfu-util.sourceforge.net/)

- **GCC (GNU Compiler Collection)**  
  GCC (version number - 8.1.0) is licensed under the GNU General Public License (GPL) Version 3.  
  GCC is used only as a build tool; your code and its output are not affected by the GCC license.  
  [GCC License Information](https://gcc.gnu.org/onlinedocs/libstdc++/manual/license.html)  

**Note:**  
We do **not** distribute these tools directly. Customers need to install it themselves. Please refer to the official sources above for license details and downloads.

---

## Building Instructions

Double click on dfu_host_utility_exe_update.bat. If there are no errors then the two executables should be copied into the <sdk path>\tools\bsl\dfu_host_utility. Any time you need to make any code changes to the source code just rerun the batch file, which will overwrite the exisiting executables.

**Note:**
- Make sure GCC is installed in the system.
- Or use your preferred Windows C compiler.

---

## Notes

- Initial setup does necessary connection and gets device info at startup.
- For programming, all address and length should be 8-byte aligned.
- By defualt VendorID and ProductID is 2047 and 0210 respectively.
- The example.bat file is a batch script that demonstrates how to use the dfu host utility to perform a typical device update workflow.

---

## Troubleshooting

- Cannot open file: Check the file path and make sure the file exists.
- DFU operation fails: Make sure your device is in DFU mode and connected.
- No response: Try unplugging and reconnecting your device, then restart the utility.
- Make sure all required `.exe` files are present and accessible.

---