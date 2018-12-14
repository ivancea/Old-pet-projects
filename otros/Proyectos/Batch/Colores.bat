@echo off
title colores
set a=0
set b=5
:e
echo JAJAJAJAJAJAJAJAJAJAJAJAJAJAJAJAJAJAJAJAJAJAJAJAJAJAJAJAJAJAJAJAJAJAJAJAJAJAJAJA
color %a%%b%
if %a%==9 goto aa
:c
if %b%==9 goto bb
set /a a=%a%+1
set /a b=%b%+1
goto e
:aa
set a=0
goto c
:bb
set b=0
goto e