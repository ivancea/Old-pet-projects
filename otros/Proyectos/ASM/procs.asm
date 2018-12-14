format PE GUI 4.0

include 'ivancea.inc'

.data
      libros db ?

.code
  main:
      call icf_init
      mov [libros],al
      stdcall prueba,200,3
      invoke Sleep,-1
      ret
.end main

proc prueba first:DWORD,second
    mov eax,[first]
    call writenumber
    ret
endp