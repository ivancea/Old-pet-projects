@echo off
msg * Hoy es dia %date:~0,2%, del mes %date:~3,2%, del ano %date:~6,4%.
set hora="Son las"
set hora2="horas"
set minutos="minutos"
if %time:~0,2%=="01" set hora="Es la"
if %hora%=="Es la" set hora2="hora"
if %time:~3,2%=="01" set minutos="minuto"
msg * %hora% %time:~0,2% %hora2% y %time:~3,2% %minutos%.
exit

