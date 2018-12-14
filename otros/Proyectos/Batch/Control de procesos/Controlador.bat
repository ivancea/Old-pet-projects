@echo off
title Controlador de procesos...
echo.
echo Dejar abierto este programa...
echo.
cd datos

set numero=0
:a
set /a numero=%numero%+1
set a=%time:~0,5%
set h=%time:~0,2%
set m=%time:~3,2%
set dia=%date:~0,2%
set mes=%date:~3,2%
set ano=%date:~6,4%
set /a asd=%dia%*%mes%*%ano%*%h%*%m%

tasklist >%asd%.icf


echo.
echo %h%:%m%  -  Procesos enviados a archivo de texto en "datos"...
echo           Se envia uno cada minuto. Esta sesion se han creado %numero% archivos.





:b
if not %a%==%time:~0,5% goto a

goto b
