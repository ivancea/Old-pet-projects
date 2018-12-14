@echo off
title Buscador de registros de procesos
:a
cls
cd datos
echo.
echo Elija el archivo a abrir poniendo su nombre sin extension:
echo.
echo.
dir
set /p a="Nombre: "
type "%a%.icf"
cls
pause
goto a
exit