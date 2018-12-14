@echo off
title Calculador de diferencias de progresiones aritmeticas y del primer termino
echo.
echo.
echo Posicion del primer termino (el mas cercano al comienzo de la progresion):
set /p posterminoa=
echo Posicion del segundo termino:
set /p posterminob=
echo Valor del primer termino:
set /p valterminoa=
echo Valor del segundo termino:
set /p valterminob=
set /a diferencia=(valterminob-valterminoa)/(posterminob-posterminoa)
set /a primertermino=valterminoa-(diferencia*(posterminoa-1))
cls
echo Termino %posterminoa% de valor %valterminoa%.
echo Termino %posterminob% de valor %valterminob%.
echo.
echo.
echo Diferencia de la progresion aritmetica:
echo %diferencia%
echo Primer termino de la progresion:
echo %primertermino%
pause Pulse una tecla para cerrar el programa...
exit