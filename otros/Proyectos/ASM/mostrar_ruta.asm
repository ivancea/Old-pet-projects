format PE GUI 4.0
include 'ivancea.inc'

.data
    var db 'Debe pasar al menos 1 parametro'
    vernum db 'Numero de argumentos en la pila: '
    feed db 13,10

.code
  main:
    call icf_init
    invoke GetCommandLine
    cmp eax,2
    jnl fin
    mov eax,var
  fin:
    call write
    invoke  Sleep,-1

.end main