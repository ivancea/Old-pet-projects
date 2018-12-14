format PE GUI 4.0

include 'ivancea.inc'
.code
    start:
	call icf_init
	mov ax, 'ab'
	mov bx, 'ab'
	cmp ax, bx

	jz kk
	mov eax, 0
	jmp endt

	kk:
	mov eax, 1
	endt:

	call writenumber

	invoke Sleep, -1
	ret
.end start