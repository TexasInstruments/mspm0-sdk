@echo off
echo "---- Building DFU host utility exe -----------------------------"

gcc hostutility.c -o ../hostutility.exe
gcc hostutility_interface.c -o ../hostutility_interface.exe
pause
