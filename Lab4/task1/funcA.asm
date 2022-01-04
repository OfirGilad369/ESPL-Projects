section .text
	global funcA
	
funcA:
	push	ebp
	push	ebx
	mov	ebp, esp
	mov	eax,-1

.L2:
	add eax, 1 ;increase the value of eax
	mov ebx, eax ;set ebx offset from the start of the input string before the reading
	add ebx, [ebp+12] ;get the next char in the input string
	movzx	ebx, BYTE [ebx] ;zero extends the value of ebx
	test bl,bl ;check if bl equals to the zero char
	jne .L2 ;loop until bl contains the zero char
	mov esp, ebp
	pop ebx
	pop ebp
	ret