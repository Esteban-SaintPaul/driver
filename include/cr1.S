;----------------------------------------------------
; Cargador de C para Silicio

[BITS 32]
global start
extern main

SECTION .text 
ALIGN 4

start:
	call	main
	mov	eax,0; SYS_EXIT
	mov	ebx,0; Salida sin error
	int	0x40
	ret

