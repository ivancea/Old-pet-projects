@echo off
title Alarma
echo Recuerde tener abierta siempre esta ventana...
set /p zz="Ponga el mensaje que desea rrecibir: "
set /p a="Ponga en modo 24 horas (hh:mm) la hora en que quieras que se te avise:"
:a
if %time:~0,5%==%a% goto b
goto a
:b
msg * Son las %a%. %zz%
sndrec32 /play /close "C:\Windows\Media\Notify.wav"
exit