@echo off
title Fake de comandos de windows
color a4
echo.
echo Comandos de Windows... -fake-
:b
set /p a="%cd% >>  "
%a%
goto b