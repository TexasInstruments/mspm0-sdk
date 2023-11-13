@echo off

set SYSCFG_PATH="C:\ti\sysconfig_1.18.0\sysconfig_cli.bat"

if not exist "%SYSCFG_PATH%" (
    echo.
    echo Couldn't find Sysconfig Tool %SYSCFG_PATH%
    echo "Update the file located at <sdk path>/tools/keil/syscfg.bat"
    echo.
    exit
)

echo Using Sysconfig Tool from %SYSCFG_PATH%
echo "Update the file located at <sdk path>/tools/keil/syscfg.bat to use a different version"

set PROJ_DIR=%~1
set PROJ_DIR=%PROJ_DIR:'=%

set SYSCFG_FILE=%~2
set SYSCFG_FILE=%SYSCFG_FILE:'=%

%SYSCFG_PATH% -o "%PROJ_DIR%.." -s "%PROJ_DIR%..\..\..\..\..\..\.metadata\product.json" --compiler keil "%PROJ_DIR%..\%SYSCFG_FILE%"
