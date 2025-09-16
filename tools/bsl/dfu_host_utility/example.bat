@echo off
REM Example batch script for DFU Host Utility: connection, unlock, readback, erase, program, verify

REM Set variables
set INPUT_FILE=inputs/sample_firmware.bin
set OUTPUT_FILE=image.dfu
set ADDRESS=0x8000
set LENGTH=0x1000
set PASSWORD_FILE=inputs/sample_password.txt
set READBACK_FILE=output/readback1.txt
set READBACK_FILE2=output/readback2.txt

REM 1. Test connection
hostutility.exe -c CMD_CONNECTION

REM 2. Unlock BSL
hostutility.exe -c CMD_UNLOCK_BSL -i %PASSWORD_FILE%

REM 3. Read back memory before erase
hostutility.exe -c CMD_MEMORY_READ_BACK -a %ADDRESS% -l %LENGTH% -o %READBACK_FILE%

REM 4. Mass erase
hostutility.exe -c CMD_MASS_ERASE

REM 5. Program device with binary file
hostutility.exe -c CMD_PROGRAM_DATA -i %INPUT_FILE% -a %ADDRESS% -o %OUTPUT_FILE%

REM 6. Read back memory after programming
hostutility.exe -c CMD_MEMORY_READ_BACK -a %ADDRESS% -l %LENGTH% -o %READBACK_FILE2%

pause