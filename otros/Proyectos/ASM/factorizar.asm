format PE GUI 4.0

include 'ivancea.inc'

.data
    acabado db 'Fin',0
    crlf db 13,10,0

.code
    main:
	call icf_init
	mov eax,1
	call writenumber
	mov eax,20h
	call writechar
	mov eax,65d  ;EAX: Numero para factorizar
	mov ecx,2     ;ECX: Numero divisor
	bucle_dividiendo:
	    xor edx,edx
	    push eax
	    div ecx
	    cmp edx,0
	    je divisor_found
	    pop eax
	    push ecx
	    mov ebx,eax
	    mov eax,ecx
	    mul ecx
	    xor eax,ebx
	    xor ebx,eax ;arreglo para recolocar EAX
	    xor eax,ebx
	    cmp ebx,eax
	    ja fin	;jump if above (unsigned comparison)
	    ;mov eax,ebx
	    pop ecx
	    cmp ecx,2
	    jne plus_two
	    inc ecx
	    jmp bucle_dividiendo
	plus_two:
	    add ecx,2
	    jmp bucle_dividiendo

	divisor_found:
	    pop ebx  ;Limpiar pila
	    push eax
	    mov eax,ecx
	    call writenumber
	    mov al,20h
	    call writechar
	    pop eax
	    cmp eax,1
	    jne bucle_dividiendo

	fin:
	    cmp eax,1
	    je equal_1
	    call writenumber
	    mov al,20h
	    call writechar
	equal_1:
	    mov eax,crlf
	    call write
	    mov eax,acabado
	    call write
	    invoke Sleep,-1

.end main