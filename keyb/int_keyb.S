[BITS 32]
global	key_dato
global	int_keyb
global c_base
global b_des

SECTION .text
int_keyb:
	push	eax
	push	edx

	mov	eax,0
	mov dword [key_dato],0		; key_dato = 0
	in	al, 0x60
	mov dword [key_dato],eax	; key_dato = int 0x60

	mov	eax, [b_des]
	mov	eax, [eax*4+c_base]
	test	eax, eax
	jne	fin_keyb		; c_base [bdes] == 0

	mov	eax, [b_des]
	mov	edx, [key_dato]
	mov	[eax*4+c_base], edx	; c_base [b_des] = key_dato

	mov	eax, [b_des]
	add	eax, 1
	mov	[b_des], eax		; b_des++

	mov	eax, [b_des]
	cmp	eax, 5
	jne	fin_keyb
	mov dword [b_des], 0
fin_keyb:
	pop	edx
	pop	eax
	ret
key_dato:	dd	0	; unsigned long key_dato;
c_base:		dd	0	; unsigned long c_base[5];
		dd	0
		dd	0
		dd	0
		dd	0
b_des:		dd	0	; unsigned long b_des;
