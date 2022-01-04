section .text
	
global _start
global read
global write
global open
global close
global strlen
global main

global utoa_s
global atou_s

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

main:
	enter 154, 0 ;reserving 150 bytes for buffer
	mov ebx, [ebp+12] ;argv
	add ebx, 4 ;ebx is now argv[1]

safe_open:
	mov ebx, [ebx] ;retrieves argv[1] data
	push dword 0 ;set int flags for function open
	push ebx ;set char *name for function open
	
	call open
	
	pop ebx ;stack maintanance
	pop ebx ;stack maintanance
	mov [ebp-154], eax ;save fd_in on stack
	cmp eax, 0 ;error check
	jle handle_error

buffer_read:
	push dword 150 ;set int size for function read
	mov ebx, ebp
	sub ebx, 150 
	push ebx ;set char *buf for function read
	push dword [ebp-154] ;set int fd for function read
	
	call read
	
	pop ebx ;stack maintanance
	pop ebx ;stack maintanance
	pop ebx ;stack maintanance
	cmp eax, -1 ;error check
	jle handle_error
	cmp eax, 0 ;buffer not empty check
	jg print

finish_reading:
	push dword [ebp-154] ;set int fd for function close
	
	call close
	
	pop ebx ;stack maintanance
	jmp return

print:
	push eax ;set int size for function write
	mov ebx, ebp
	sub ebx, 150
	push ebx ;set char *buf for function write
	push dword 1 ;set int fd for function write (stdout)

	call write

	pop ebx ;stack maintanance
	pop ebx ;stack maintanance
	pop ebx ;stack maintanance

	cmp eax, 0 ;error check
	jle handle_error
	jmp buffer_read

handle_error:
	mov eax, 1 
	leave ;returning reserved bytes
	ret

return:
	mov eax, 0
	leave ;returning reserved bytes
	ret

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

utoa_s:
    push ebp
	push ebx
	mov	ebp, esp
    mov [ebp-4], dword 10 ;construct the devisor
    mov eax, [ebp+12] ;get argument number
    mov ebx, [ebp+16] ;get argument buffer
    mov [ebp-8], ebx ;save buffer address
    mov ebx, [ebp-8] ;restore buffer pointer to start

.zero_case_handler:
	mov edx, 0 ;construct edx to store the remainder
    div dword [ebp-4] ;divide the number by 10, eax will store quotient, edx will store remainder
    mov cl, dl ;get remainder
    or edx, eax ;check if both the quotient
    je .zero_case
    jmp .save_char
    
.zero_case:
	add cl, '0'
	mov [ebx], cl ;write remainder to buffer
    inc ebx ;move buffer pointer to the next cell
    jmp .finish_convert

.div_number:
    mov edx, 0 ;construct edx to store the remainder
    div dword [ebp-4] ;divide the number by 10, eax will store quotient, edx will store remainder
    mov cl, dl ;get remainder
    or edx, eax ;check if both the quotient
    jne .save_char 
    jmp .finish_convert

.save_char:
    add cl, '0' ;convert to char
    mov [ebx], cl ;write remainder to buffer
    inc ebx ;move buffer pointer to the next cell
    jmp .div_number

.finish_convert:
    mov [ebx], byte 0 ;close the str with an \0
    dec ebx ;move ebx to point on the cell before \0
    mov ecx, [ebp-8] ;move ecx to point at the start of buffer

.reverse_result:
    cmp ecx, ebx ;check if ecx is pointing to the same address as ebx or after it
    jge FINISH

.exchange_chars:
    mov al, [ebx] ;exchange the bytes between ebx and ecx to reverse the output string
    xchg al, [ecx]
    mov [ebx], al
    dec ebx ;move pointers, decrease ebx and increase ecx
    inc ecx
    jmp .reverse_result

atou_s:
	push ebp
	push ebx
	mov	ebp, esp
	mov edx, [ebp+12] ;get argument string
	mov eax, 0 ;construct eax to store the result
	
.L3:
	movzx ecx, byte [edx] ;get next char
	inc edx ;increase index for reading next char
	cmp ecx, '0' ;checking validation (digit bigger then 0)
	jb FINISH
	cmp ecx, '9' ;checking validation (digit smaller then 9)
	ja FINISH
	sub ecx, '0' ;converting the char to number
	imul eax, 10 ;multiply the current number by 10
	add eax, ecx ;add the current digit to the number
	jmp .L3 ;loop until done

FINISH:
	mov esp, ebp
	pop ebx
	pop ebp
	ret
