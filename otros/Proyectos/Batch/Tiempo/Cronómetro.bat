@echo off
title Cronometro
echo Mantenga esta ventana abierta
echo Ponga el tiempo en que sera avisado...[hh-mm-ss]
:principio
set /p ccc="Ejemplo: 05-26-01.  Mal: 5-26-1.  Max: 99-99-99:   "
set ccc=%ccc%-
goto comprobar
:comprobado

set /a h=%time:~0,2% + %ccc:~0,2%
set /a m=%time:~3,2% + %ccc:~3,2%
set /a s=%time:~6,2% + %ccc:~6,2%
echo Tic tac...




:bucle
if %time:~0,8%==%h%:%m%:%s% goto msg
goto bucle








:msg
msg * Ya han transcurrido %ccc:~0,2% horas, %ccc:~3,2% minutos, %ccc:~6,2% segundos. Son las %time:~0,8%.
exit


:comprobar
if not %ccc:~2,1%==- goto principio
if not %ccc:~5,1%==- goto principio
if %ccc:~7,1%==- goto principio
cls
goto comprobado
