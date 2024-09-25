@echo off

set SYSCFG_PATH="C:\ti\sysconfig_1.19.0\sysconfig_cli.bat"

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

:: Search for the root of the SDK by going up one directory
:: However, if we don't find it after 20 times then give up
set SDK_ROOT=%PROJ_DIR%
set iter=0
:up_one
if exist "%SDK_ROOT%\.metadata\product.json" (
    goto found
) else if %iter% geq 20 (
	@echo "Couldn't find .metadata\product.json"
) else (
	set /a iter=%iter%+1
	set SDK_ROOT=%SDK_ROOT%..\
	goto up_one
)
:found

%SYSCFG_PATH% -o "%PROJ_DIR%.." -s "%SDK_ROOT%\.metadata\product.json" --compiler keil "%PROJ_DIR%..\%SYSCFG_FILE%"
