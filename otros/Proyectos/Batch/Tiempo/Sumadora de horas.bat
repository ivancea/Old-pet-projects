@echo off
title Sumadora de horas
:principio
echo Escriba cualquier caracter si quiere sumarle a la hora actual, o 1 si quiere sumar dos horas.
set /p abcd="Numero:  "
if %abcd%==1 goto horas
cls



:principio1
cls
echo Ponga cuantas horas, minutos y segundos le quieres sumar a la hora actual [hh-mm-ss]
set /p ccc="Ejemplo: 05-26-01.  Mal: 5-26-1.  Max: 99-99-99:   "
set ccc=%ccc%-
goto comprobar1
:comprobado1
set /a h=%time:~0,2% + %ccc:~0,2%
set /a m=%time:~3,2% + %ccc:~3,2%
set /a s=%time:~6,2% + %ccc:~6,2%
:s
if %s% GEQ 60 goto sa
:m
if %m% GEQ 60 goto ma
:h
if %h% GEQ 24 goto ha

cls 
echo Hora real: %time:~0,8%
echo Hora mas %ccc:~0,8% horas: %h%:%m%:%s%
pause
exit


:sa
set /a m=%m%+1
set /a s=%s%-60
goto s
:ma
set /a h=%h%+1
set /a m=%m%-60
goto m
:ha
set /a h=%h%-24
goto h



:horas
cls

echo Ponga cuantas horas, minutos y segundos quiere sumar [hh-mm-ss]
set /p bbb="Ejemplo: 05-26-01.  Mal: 5-26-1.  Max: 99-99-99:   "
set bbb=%bbb%-
goto comprobar2
:comprobado2
cls
echo Ponga cuantas horas, minutos y segundos quiere sumarle a la hora [hh-mm-ss]
set /p tatata="Ejemplo: 05-26-01.  Mal: 5-26-1.  Max: 99-99-99:   "
set tatata=%tatata%-
goto comprobar3
:comprobado3
set /a h=%tatata:~0,2% + %bbb:~0,2%
set /a m=%tatata:~3,2% + %bbb:~3,2%
set /a s=%tatata:~6,2% + %bbb:~6,2%
:ss
if %s% GEQ 60 goto sb
:mm
if %m% GEQ 60 goto mb
:hh
if %h% GEQ 24 goto hb

cls
echo Hora: %tatata:~0,8%
echo Hora mas %bbb:~0,8% horas: %h%:%m%:%s%
pause
exit


:sb
set /a m=%m%+1
set /a s=%s%-60
goto ss
:mb
set /a h=%h%+1
set /a m=%m%-60
goto mm
:hb
set /a h=%h%-24
goto hh


:comprobar1
if not %ccc:~2,1%==- goto principio1
if not %ccc:~5,1%==- goto principio1
if %ccc:~7,1%==- goto principio1
cls
goto comprobado1

:comprobar2
if not %bbb:~2,1%==- goto horas
if not %bbb:~5,1%==- goto horas
if %bbb:~7,1%==- goto horas
cls
goto comprobado2
:comprobar3
if not %tatata:~2,1%==- goto comprobado2
if not %tatata:~5,1%==- goto comprobado2
if %tatata:~7,1%==- goto comprobado2
cls
goto comprobado3