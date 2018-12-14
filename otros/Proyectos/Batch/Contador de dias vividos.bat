@echo off
title Días desde el nacimiento
:started
cls
echo.
echo Ponga todo con el formato "XX-XX-XXXX". No olvide digitos.
echo.
set /p d="Dia de nacimiento: "
set /p m="Mes de nacimiento: "
set /p a="Ano de nacimiento: "

if %d% gtr 31 goto error
if %m% gtr 12 goto error
if %a% gtr %date:~6,4% goto error

set dnac=%d%
set mnac=%m%
set anac=%a%

if not %d% gtr %date:~0,2% goto 1
set /a m=%m%-1
set /a d=%d%+31
:1                                  
                                    
if not %m% gtr %date:~3,2% goto 2   
set /a a=%a%-1
set /a m=%m%+12
:2

set /a dtot=%date:~0,2%-%d% + (%date:~3,2%-%m%)*31 + (%date:~6,4%-%a%)*365
set /a xxx=%date:~3,2%-%m%
if "%xxx%" gtr 11 (set /a %dtot%=%dtot%-7)

set /a dtot= %dtot%+(%date:~6,4%-%a%)/4
cls
echo.
echo Hoy es %date%. Usted ha nacido el %dnac%/%mnac%/%anac%.
echo.
echo.
echo Has cumplido %dtot% dias desde que naciste.
echo.
echo.
echo Rango de error de 7.
pause>nul
exit

:error   
echo.
echo Error en los numeros introducidos...
echo.
pause
goto started