format PE GUI 4.0
include 'ivancea.inc'
.code
  start:
    call icf_init
    mov eax,0	;a
    mov ebx,1	;b
    mov ecx,1	;c
    mov edx,0	;suma de pares
    main_loop:

       push ecx
       mov ecx,ebx
       mov eax,ebx
       pop ebx
       add ecx,ebx

       push eax
       push ecx
       push edx
       mov eax,ecx
       mov ecx,2
       xor edx,edx
       div ecx	;mira si es par
       cmp edx,0
       jnz et_1

       pop edx	;si es par...
       pop ecx
       pop eax
       add edx,ecx
       cmp ecx,4000000d
       jl et_2
       sub edx,ecx
       jmp main_fin
     et_1:
       pop edx	 ;si no es par...
       pop ecx
       pop eax
     et_2:
       cmp ecx,4000000d
    jl main_loop
    main_fin:
    mov eax,edx
    call tostring
    mov eax,edx
    call write
    invoke Sleep,-1
.end start   