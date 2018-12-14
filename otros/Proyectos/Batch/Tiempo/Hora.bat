@echo off
title "Hora"
color 1a
mode con cols=15 lines=2
:a
cls
set a=%time:~0,8%
echo %a%
:b
if not %a%==%time:~0,8% goto a
goto b