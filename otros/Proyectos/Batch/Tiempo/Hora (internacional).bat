@echo off
title Hora
:a
set a=%time:~0,8%
set /a bh=%time:~0,2%-1
if %bh%==0 goto z
goto x
:z
set bh=23
:x
set bm=%time:~2,6%
cls
echo Hora segun el meridiano de Greenwich: %bh%%bm%
echo.
echo Hora Española (+1): %a%
:b
if not %a%==%time:~0,8% goto a
if not %bm%==%time:~2,6% goto a
goto b