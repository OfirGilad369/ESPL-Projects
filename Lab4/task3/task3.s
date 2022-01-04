%define X 'A'
%define STR string1


section .rodata
    prefix db 'My character is', 0
    format db '%s %c', 10, 0
    print_format db '"%c" appears in "%s" %d times', 10, 0
    string1 db 'ABBA', 0
    string2 db 'BBA', 0
    string3 db 'BB', 0
    string4 db '', 0

section .text
    global _start
    extern printf

_start:
    sub esp, 4 ; allocate stack space for printf result
    mov eax, 0 ;set eax as counter
    mov ebx, STR ;set ebx as the index from where to start reading
    
.L2:
	mov ecx, [ebx] ;set ecx as the next char in string
	test cl,cl ;check if zero char was found
	je .next ;end of string found
	cmp cl, X ;compare between X and a char of the string
	je .increase_counter ;a char equal to X was found
	add ebx, 1 ;set ebx to point on the next char of the string
	jmp .L2 ;loop back if not reached end of string
	
.increase_counter:
	add eax, 1 ;increase the counter eax as a char equal to X has been found
	add ebx, 1 ;skip to the next char of the string
	jmp .L2 ;jump back to the loop until the end of the string will be found
 
.next:
    push eax ; argument 4 to printf
    push STR ; argument 3 to printf
    push X ; argument 2 to printf
    push print_format ; argument 1 to printf
    call printf
    add esp, 12 ; return the stack pointer to it's original location

exit:
    mov ebx, 0
    mov eax, 1
    int 80h
