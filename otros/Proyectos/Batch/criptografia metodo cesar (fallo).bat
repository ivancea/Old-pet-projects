@echo off
setlocal enableDelayedExpansion
::Frase a convertir
set w=hgoelspdio           
set local=0
set abecedario=abcdefghijklmnñopqrstuvwxyza
set final=0
::"IN  (0,1,XX)  ->  cambiar la XX por al longitud de la cadena a convertir
FOR /L %%x IN (0,1,10) DO (
pause
set www=0
set p=%%x
:f
echo !abecedario:~%www%,1! -- !w:~%p%,1!
pause
if !abecedario:~%www%,1!==!w:~%p%,1! (goto ya)
set /a www=%www%+1
goto f

:ya
set local=%abecedario:~%www%,1%
set final=%final%+%local%
echo %local% 
)          

echo %final%
pause