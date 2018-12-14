@echo off
title Prueba
:a
cls
echo.
echo Elige color:
echo.
echo 1: azul y verde
echo 2: amarillo y negro
echo.
echo.
set /p a="Numero: "
if %a%==1 (datos.bat 1&pause&goto a)
if %a%==2 (datos.bat 2&pause&goto a)
echo.
echo Elige 1 o 2.
goto a
exit