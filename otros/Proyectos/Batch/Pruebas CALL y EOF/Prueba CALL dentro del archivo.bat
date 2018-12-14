@echo off
title Prueba de CALL dentro del archivo
:a
cls
echo.
set /p c="Numero de columnas que tendra el cmd: "
set /p f="Numero de filas que tendra el cmd: "
call:mode
goto a

:mode
mode con cols=%c% lines=%f%
