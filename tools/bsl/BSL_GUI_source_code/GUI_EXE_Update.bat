@echo off
echo "---- Building MSPM0 BSL GUI -----------------------------"

setlocal
set workdir=%CD%
echo.

python -m venv "%workdir%"\venv
call "%workdir%"\venv\Scripts\activate.bat
pip install -r requirements.txt
pyinstaller --specpath "%workdir%"\opt\spec --distpath "%workdir%"\opt\dist --workpath "%workdir%"\opt\build -F "%workdir%"\MSPM0_BSL_GUI.py -w
copy "%workdir%"\opt\dist\MSPM0_BSL_GUI.exe "%workdir%"\..\BSL_GUI_EXE\MSPM0_BSL_GUI.exe
pause
