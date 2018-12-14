@echo off
title PRUEBAS
echo %0
echo.
echo.
echo Ruta al archivo: %~f0
echo Disco: %~d0
echo Solo ruta: %~p0
echo Nombre: %~n0
echo Extension: %~x0
echo Ruta Corta: %~s0
echo Atributos: %~a0
echo Fecha: %~t0
echo Tamaño: %~z0
echo Fecha + tamaño: %~tz0
echo.
echo.
set x=252
echo %x%
echo %%x%%
echo %%%x%%%
echo.
echo.

echo hola
echo %errorlevel%
cd C:\bibibib
echo %errorlevel%
echo.
echo.
echo Estoy en: %~nx0-^>%0 %*
pause