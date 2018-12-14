@echo off
:: esto sirve para que no se muestren las lineas de comando en la ventana del ms-dos.
:: los :: sirven para anotar algo que no quieres que se muestre, como por ejemplo, estas explicaciones que estoy dejando.Se puede poner :: o rem.
title Apagar/reiniciar
:: el comando title pondra el título de la ventana.
echo Bienvenido al programador de apagado.
:: echo ??? sirve para poner un mensaje que se mostrara en la ventana.
echo. 
:: echo. sirve para dejar una linea en blanco.
pause
:: al poner pause, el usuario deberá pulsar una tecla para continuar con los comandos, y aparecerá el mensaje que pongas delante (pause el-mensaje) o, por defecto, "pulse una tecla para continuars. También se puede poner pause >nul, para que no muestre ningún mensaje (el problema es que el usuario no va a saber que hacer, a no ser que lo especifiques antes, con "echo", por ejemplo.
:1
:: al poner :-algo- (-algo- en este caso es 1) creas una zona, de tal modo que si en algún momento del código, antes o despues de esa zona, pones "goto -algo", el comando volverá a esa zona, repitiendo, o saltando parte del código.
cls
:: el comando cls deja en blanco todo lo que había en la ventana, mensajes, etz.
echo Elije que deseas hacer: (pon el número)
echo 1) Apagar, reiniciar.
echo 2) Salir.
echo.
echo.
echo.
set /p numero=Numero que hallas elejido: 
:: el comando "set" con el parámetro "/p" crea una variable, donde "numero" es el nombre de la variable (lo que va después de /p). Lo que hay despues del = es un mensaje, delante del cual el usuario escribirá lo que desé que sea la variable.
if %numero%==1 goto 2
if %numero%==2 goto salir
:: los comandos "if %variable%==" sirve para decir: si la variable %variable% (siempre entre %%) es igual a X, iniciar X comando, goto, en este caso.
cls
echo Por favor, poner 1 o 2, según lo que desé.
pause pulse una tecla para volver al menu inicial.
goto 1
:: aquí el comando goto que nos enviará a la "zona" llamada 1.
:salir
exit
:: el comando exit cierra la ventana.
:2
:: otra zona más.
cls
echo apagar (a) o reiniciar (r)?
set /p ar=Letra:
if %ar%==a goto apagar
if %ar%==r goto reiniciar
cls
echo por favor, elija "a" o "r".
pause
goto 2
:: por supuesto, pueden ser colocados todos los goto que se quieran para ir a la misma zona.
:apagar
cls
echo quiere apagar en X tiempo (t) o directamente (d)?
set /p apagar2=Letra:
if %apagar2%==t goto apagart
if %apagar2%==d goto apagard
cls
echo Por favor, elija una de las opciones.
pause
goto apagar
:apagart
cls
echo bien, especifique el tiempo en segundos en que quiera apagar (máx. 600 min. 1) y prepárese.
set /p tiempoapagar=Tiempo:
shutdown -s -t %tiempoapagar%
:: shutdown sirve para apagar (-s), reiniciar (-r), hibernar, etz. tambien se puede poner variables en partes de los comandos. Si ponemos -t # podremos especificar el tiempo en segundos en que se apagará/reiniciará/etz.
exit
:apagard
echo preparese.
pause pulse una tecla para apagar.
shutdown -s -t 1
:: aunque pongo directamente, pongo -t 1 porque hay versiones de sistemas operativos que, si no pones tiempo o hora (también se puede poner la hora a la que hacer shutdown), pone 30 segundos.
exit
:reiniciar
cls
echo quiere reiniciar en X tiempo (t) o directamente (d)?
set /p reiniciar2=Letra:
if %reiniciar2%==t goto reiniciart
if %reiniciar2%==d goto reiniciard
cls
echo Por favor, elija una de las opciones.
pause
goto reiniciar
:reiniciart
cls
echo bien, especifique el tiempo en segundos en que quiera reiniciar (máx. 600 min. 1) y prepárese.
set /p tiemporeiniciar=Tiempo:
shutdown -r -t %tiemporeiniciar%
exit
:reiniciard
echo preparese.
pause pulse una tecla para reiniciar.
shutdown -r -t 1
exit
::Hecho por Iván Cea. Todos los derechos reservados./Made by Iván Cea. All right reserved.
