@echo off
title Programa de ping
:start
cls
echo.
echo Por favor, escriba aquí el nombre de la página web.
echo Recuerde poner WWW.---.net/com/es/org/etz
echo.
echo.
echo.
echo.
echo.
echo.
set /p a="Quiero hacer ping a: "
cls
echo Haciendo ping a %a%...   ...   ...
ping %a%
pause
exit
