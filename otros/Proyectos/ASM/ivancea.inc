include 'win32ax.inc'

  icf_init:
	invoke	AllocConsole
	ret
  malloc: ; EAX: Size  //  Retorna: EAX: Puntero a string
	push ebx
	push ecx
	push edx
	invoke	VirtualAlloc,NULL,eax,MEM_COMMIT+MEM_RESERVE,PAGE_READWRITE
	pop edx
	pop ecx
	pop ebx
	ret

  free: ; EAX: Address
	pusha
	invoke	VirtualFree,eax,0,MEM_RELEASE
	popa
	ret

  read: ; EAX: Size  //  Retorna: EAX: Puntero a string
	push edx
	push ecx
	mov edx,eax
	mov eax,4
	call malloc
	mov ecx,eax
	mov eax,edx
	call malloc
	push eax
	invoke ReadConsoleA,<invoke GetStdHandle,STD_OUTPUT_HANDLE>,eax,edx,ecx,0
	pop eax
	pop ecx
	pop edx
	ret

  write: ; EAX: String
	push ECX
	push EDX
	push EAX
	mov edx,eax
	xor ecx,ecx ;en ECX se guardará el tamaño de la string
	dec edx  ;para compensar el inc que sobra en el loop
	write_loop_1:
	   inc edx
	   inc ecx
	   cmp byte[edx],0 ;hasta que el char de la string sea 0
	jnz write_loop_1
	dec ecx  ;se elimina el contador sobrante.
	invoke	WriteConsole,<invoke GetStdHandle,STD_OUTPUT_HANDLE>,eax,ecx,1,0
	pop EAX
	pop EDX
	pop ECX
	ret

   writechar: ; AL: Char
	test eax,0FFh
	jz writechar_fin
	pusha
	and eax,0FFh
	push eax
	mov eax,2
	call malloc
	inc eax
	mov byte[eax],0
	dec eax
	pop ebx
	mov byte[eax],bl
	invoke	WriteConsole,<invoke GetStdHandle,STD_OUTPUT_HANDLE>,eax,1,1,0
	call free
	popa
	writechar_fin:
	ret

   writenumber: ; EAX: Number
	call tostring
	call write
	call free
	ret

   writebasenumber: ; EAX: Number   EBX: Base
	call tobasestring
	call write
	call free
	ret

   pow: ; EAX: Numero , EDX: Exponente
	push ECX
	push EDX
	mov ecx,eax
	mov eax,1
	cmp edx,0
	jnz pow_loop_1
	mov eax,1
	pop edx
	pop ecx
	ret
	pow_loop_1:
	   dec edx
	   push edx
	   xor edx,edx
	   mul ecx
	   pop edx
	   cmp edx,0
	jnz pow_loop_1
	pop edx
	pop ecx
	ret

   geteip: ; Retorna: EAX: EIP
	mov eax,[esp]
	ret

   tostring: ; EAX: Numero  //	Retorna: EAX: Puntero a string
	push EDX
	push ECX
	push EBX
	mov ebx,eax
	xor ecx,ecx ;Aqui guardo al longitud de la string
	cmp eax,0
	jz tostring_ifzero
	push ebx
	mov ebx,10d
	tostring_loop_1:
	   inc ecx
	   xor edx,edx
	   div ebx
	   cmp eax,0
	jnz tostring_loop_1
	pop ebx
	mov eax,ecx
	call malloc
	mov edx,eax
	mov eax,ebx
	mov ebx,edx
	push ebx
	add ebx,ecx
	mov ecx,ebx
	pop ebx
	push ebx
	mov ebx,10d
	tostring_loop_2:
	   dec ecx
	   xor edx,edx
	   div ebx
	   mov byte[ecx],dl
	   add byte[ecx],30h
	   cmp eax,0
	jnz tostring_loop_2
	pop ebx
	mov eax,ebx
	pop EBX
	pop ECX
	pop EDX
	ret
	tostring_ifzero:
	   mov eax,01h
	   call malloc
	   mov byte[eax],30h
	   pop EBX
	   pop ECX
	   pop EDX
	   ret

   tobasestring: ; EAX: Numero	EBX: Base  //  Retorna: EAX: Puntero a string
	push EDX
	push ECX
	cmp ebx,2
	jnl tobasestring_next
	mov ebx,10d
	jmp tobasestring_ebx_good
	tobasestring_next:
	cmp ebx,10d
	jng tobasestring_ebx_good
	mov ebx,10d
	tobasestring_ebx_good:
	push EBX
	mov ebx,eax
	xor ecx,ecx ;Aqui guardo al longitud de la string
	cmp eax,0
	jz tobasestring_ifzero
	xor ebx,[esp]
	xor [esp],ebx
	xor ebx,[esp]
	;push ebx
	;mov ebx,10d
	tobasestring_loop_1:
	   inc ecx
	   xor edx,edx
	   div ebx
	   cmp eax,0
	jnz tobasestring_loop_1
	xor ebx,[esp]
	xor [esp],ebx
	xor ebx,[esp]
	;pop ebx
	mov eax,ecx
	call malloc
	mov edx,eax
	mov eax,ebx
	mov ebx,edx
	push ebx
	add ebx,ecx
	mov ecx,ebx
	pop ebx
	xor ebx,[esp]
	xor [esp],ebx
	xor ebx,[esp]
	;push ebx
	;mov ebx,10h
	tobasestring_loop_2:
	   dec ecx
	   xor edx,edx
	   div ebx
	   mov byte[ecx],dl
	   add byte[ecx],30h
	   cmp eax,0
	jnz tobasestring_loop_2
	xor ebx,[esp]
	xor [esp],ebx
	xor ebx,[esp]
	;pop ebx
	mov eax,ebx
	pop EBX
	pop ECX
	pop EDX
	ret
	tobasestring_ifzero:
	   mov eax,01h
	   call malloc
	   mov byte[eax],30h
	   pop EBX
	   pop ECX
	   pop EDX
	   ret