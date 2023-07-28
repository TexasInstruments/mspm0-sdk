@echo off
IF [%1] == [--all] GOTO CLEAN_ALL
fs --clean --prompt
GOTO END

:CLEAN_ALL
fs --cleanall --prompt

:END
