section .text
	
global _start
global read
global write
global open
global close
global strlen

extern main
_start:

	;TODO: handles command line input parameters
	
	mov eax, [esp] ;argc
	mov ebx, esp ;argv, since argv is in: esp+4 we add 4 in the next line
	add ebx, 4
	
	push ebx ;push argv to stack
	push eax ;push argc to stack
	
	call	main
    mov     ebx,eax
	mov	eax,1
	int 0x80

read:
	push ebp
	push ebx
	mov	ebp, esp
	mov eax, 3 ;Sys_read system call number
	mov ebx, [ebp+12] ;retrieves the first function argument
	mov ecx, [ebp+16] ;retrieves the second function argument
	mov edx, [ebp+20] ;retrieves the third function argument
	int 0x80
	jmp FINISH

write:
	push ebp
	push ebx
	mov	ebp, esp
	mov eax, 4 ;Sys_write system call number
	mov ebx, [ebp+12] ;retrieves the first function argument
	mov ecx, [ebp+16] ;retrieves the second function argument
	mov edx, [ebp+20] ;retrieves the third function argument
	int 0x80
	jmp FINISH

open:
	push ebp
	push ebx
	mov	ebp, esp
	mov eax, 5 ;Sys_open system call number
	mov ebx, [ebp+12] ;retrieves the first function argument
	mov ecx, [ebp+16] ;retrieves the second function argument
	mov edx, 0777 ;read, write and execute by all
	int 0x80
	jmp FINISH

close:
	push ebp
	push ebx
	mov	ebp, esp
	mov eax, 6 ;Sys_close system call number
	mov ebx, [ebp+12] ;retrieves the first function argument
	int 0x80
	jmp FINISH

strlen:
	push ebp
	push ebx
	mov	ebp, esp
	mov	eax,-1
	jmp .L2

.L2:
	add eax, 1 ;increase the value of eax
	mov ebx, eax ;set ebx offset from the start of the input string before the reading
	add ebx, [ebp+12] ;get the next char in the input string
	movzx ebx, BYTE [ebx] ;zero extends the value of ebx
	test bl,bl ;check if bl equals to the zero char
	jne .L2 ;loop until bl contains the zero char
	jmp FINISH

FINISH:
	mov esp, ebp
	pop ebx
	pop ebp
	ret
