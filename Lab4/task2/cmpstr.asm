section .text
	global cmpstr
	
cmpstr:
	push ebp
	push ebx
	mov	ebp, esp
	mov	eax,-1 ;set eax as the index from where to start reading

.L2:
	add eax, 1 ;increase the value of eax
	mov ebx, eax ;set ebx offset from the start of string1 before the reading
	mov ecx, eax ;set ecx offset from the start of string2 before the reading
	add ebx, [ebp+12] ;get the next char in string1
	add ecx, [ebp+16] ;get the next char in string2
	movzx ebx, BYTE [ebx] ;zero extends the value of ebx
	movzx ecx, BYTE [ecx] ;zero extends the value of ecx
	test bl,bl ;check if bl equals to the zero char
	je .compare ;end of string1 found
	test cl,cl ;check if cl equals to the zero char
	je .compare ;end of string2 found
	cmp ebx, ecx ;compare between char1 to char2 of the strings
	jne .compare ;different chars were found
	jmp .L2 ;loop back if no difference found
	
.compare:
	mov eax, ebx ;set eax to get the current char where the loop ended
	sub eax, ecx ;compare between the last chars that were read before the loop ended
	jmp FINISH ;jump to the end of the program
	
FINISH:
	mov esp, ebp
	pop ebx
	pop ebp
	ret
