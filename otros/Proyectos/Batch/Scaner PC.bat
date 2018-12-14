@echo off
title Scaner PC
echo Escanearemos su pc...
pause Pulse una tecla para comenzar el escaner
:3333
set a=1
:start
echo %a% archivos escaneados
set /a a=%a%+1
if %a%==14624 goto ya
goto start
:ya
cls
echo Escaner completado.
echo.
echo RESULTADOS DEL ANALISIS:
echo.
echo.
echo 0 amenazas en "discos extraibles"
echo 0 amenazas en "puertos usb"
echo 1 amenaza en "disco local C"
echo 0 amenazas en "otras unidades de almacenamiento masivo"
echo.
pause Pulse una tecla para obtener informacion de las amenazas "disco local c"
cls
:start2
echo "error en la configuracion de %userprofile%\desktop"
pause
echo.
echo.
echo ¿Desea reparar manualmente el error o mediante la configuracion avanzada de este programa?
echo.
echo 1. Cerrar el programa y reparar manualmente.
echo 2. Reparar con la configuracion de este programa.
echo 3. Rehacer el analisis.
echo 4. Salir.
echo.
set /p como="Ponga el numero que desee: "
if %como%==1 goto 11
if %como%==2 goto 22
if %como%==3 goto 33
if %como%==4 goto 44
cls
echo Por favor, seleccione uno de los números anteriores, cierre el programa o/y repita el escaneo.
echo.
goto start2
:33
cls
pause Pulse una tecla para volver a comenzar con el analisis.
goto 3333
:44
echo Gracias por haber utilizado nuestro escaner gratuito.
pause Pulse una tecla para salir.
exit
:11
cls
echo Gracias por usar este programa, estaremos siempre a su servicio.
echo.
pause Pulse una tecla para cerrar Scaner PC.
exit
:22
cls
echo Comenzaremos con el proceso automatizado.
echo.
pause Pulse una tecla para comenzar la reparacion.
cls
echo REPARANDO...
echo Detectando problemas
echo Obteniendo base de datos .dll
echo Obteniendo información del producto
echo Obteniendo información adicional de hardware
echo Comenzando el cambio de bases
echo Realizando los cambios .dll necesarios
echo Creando informe
echo Revisando perdidas de datos
echo Reparando perdidas importantes
echo Rescatando informe de errores
echo Error en el rescate
echo Error 365874
echo Volviendo a posicionar antiguos archivos .dll
echo Recopilando informacion del error
echo Buscando informacion del error
echo.
echo.
echo.
echo Colocando informacion antigua
echo.
echo Proceso finalizado: codigo de error 365874
echo.
echo.
:mamamama
echo ¿Que desea hacer?
echo.
echo 1. Reintentar la reparacion.
echo 2. Rehacer el escaner.
echo 3. Obtener informacion avanzada sobre el codigo de error 365874
echo 4. salir.
set /p nnnn="Seleccione un numero: "
if %nnnn%==1 goto 22
if %nnnn%==2 goto 33
if %nnnn%==3 goto 9
if %nnnn%==4 goto 44
cls
echo Por favor, seleccione una de las opciones que se le presentan.
Pause Pulse una tecla para volver al ultimo menu.
goto mamamama
:9
cls
echo Obteniendo informacion sobre el error...
echo.
start echo Codigo de error 365874: Error desktop ("%userprofile%\desktop") Formato de error deconocido. Sin recomendaciones. Para mas informacion, consulte en la web o en un asistente de %os%. Perdone las molestias.
echo.
pause Pulse una tecla para volver al ultimo menu.
goto mamamama