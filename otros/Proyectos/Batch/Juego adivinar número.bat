@echo off
echo.
echo Adivina el numero!!!
echo.
echo.
set abc=%random%
set contador=0
:a
set /p respuesta="Diga un numero:"
set /a contador=%contador% + 1
if %respuesta% LSS %abc% goto b
if %respuesta% GTR %abc% goto c
if %respuesta%==%abc% goto d
:b
cls
echo El numero es mas grande que %respuesta%!!!
echo Intentos: %contador%
color 21
goto a
:c
cls
echo El numero es mas pequeno que %respuesta%!!!
echo Intentos: %contador%
color 12
goto a
cls
:d
cls
echo Exacto, el numero era %respuesta%!!!
echo Intentos: %contador%
pause
exit