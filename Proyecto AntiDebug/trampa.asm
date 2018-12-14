format PE GUI 4.0

include 'ivancea.inc'

.data
    stringPrueba db 'Bien',0

.code
    main:
	call icf_init
	xor eax,eax
	call trap
	nop
	mov eax,stringPrueba
	call write
	invoke Sleep,-1

    trap:
	inc DWORD [esp]
	ret

.end main      