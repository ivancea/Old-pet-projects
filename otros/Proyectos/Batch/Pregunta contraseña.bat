@echo off
:principio

shutdown -s -f -t "700"
set ccc=00-00-10

set /a h=%time:~0,2% + %ccc:~0,2%
set /a m=%time:~3,2% + %ccc:~3,2%
set /a s=%time:~6,2% + %ccc:~6,2%
:s
if %s% GEQ 60 goto sa
:m
if %m% GEQ 60 goto ma
:h
if %h% GEQ 24 goto ha
echo No cerrar...

:bucle
if %time:~0,8%==%h%:%m%:%s% goto c
goto bucle
:c
set /p a="Contraseña: "
if %a%==contra (shutdown -a) (else goto c)
goto principio



:s
if %s% GEQ 60 goto sa
:m
if %m% GEQ 60 goto ma
:h
if %h% GEQ 24 goto ha




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