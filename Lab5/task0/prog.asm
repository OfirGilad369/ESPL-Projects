section .text
	global open ;make open transparent to other modules
	global close ;make open transparent to other modules

open:
	push ebp ;stack maintenance
	push ebx
	mov ebp, esp ;stack maintenance
	
open_sys_call:
	mov eax, 5 ;Sys_open system call number
	mov ebx, [ebp+12] ;retrieves the first function argument
	mov ecx, [ebp+16] ;retrieves the second function argument
	mov edx, 0777 ;read, write and execute by all
	int 0x80
	mov [fd_in], eax ;save file pointer in the bss
	jmp FINISH
	
close:
	push ebp ;stack maintenance
	push ebx
	mov ebp, esp ;stack maintenance

close_sys_call:
	mov eax, 6 ;Sys_close system call number
	mov ebx, [fd_in] ;retrieves data in fd_in
	int 0x80
	jmp FINISH
	
FINISH:
	mov esp, ebp ;stack maintenance
	pop ebx
	pop ebp ;stack maintenance
	ret ;stack maintenance
	
section .bss
	fd_in resb 1
