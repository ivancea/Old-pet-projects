@echo off
title Encriptador de palabras
echo.
echo.
set /p a="Palabra a encriptar: "
set a=%a:a=4%
set a=%a:e=3%
set a=%a:i=1%
set a=%a:o=0%
set a=%a:s=5%
set a=%a:m="|\/|"%
set a=%a:u="|_|"%
set a=%a:n="|\|"%

echo.
echo.
echo Palabra encriptada:
echo.
echo %a%
pause >nul