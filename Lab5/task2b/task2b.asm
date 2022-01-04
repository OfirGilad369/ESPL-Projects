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
global cmpstr
global load_string_to_buffer
global cmpstr_in_text

section .bss
    int_to_str_buffer: resb 50 ;allocated memory for the input counter
    cmp_str: resb 50 ;allocated memory for the input string in argv[2] on case ws

section .data
	w_opt: db '-w', 0
    ws_opt: db '-ws', 0
	end_line: db 10
	count_words_str: db 'Number of words: ', 0
    count_words_str_len: equ  $ - count_words_str
    
    count_words_string_str_1: db 'Number of times the string: "', 0
    count_words_string_str_1_len: equ  $ - count_words_string_str_1
    
    count_words_string_str_2: db '" appeared on the file: ', 0
    count_words_string_str_2_len: equ  $ - count_words_string_str_2
    
    open_file_error: db 'Failed to open the file', 10, 0
    open_file_error_len: equ  $ - open_file_error

    close_file_error: db 'Failed to close the file', 10, 0
    close_file_error_len: equ  $ - close_file_error

    read_from_file_error: db 'Failed to read from file', 10, 0
    read_from_file_error_len: equ  $ - read_from_file_error

    write_to_file_error: db 'Failed to write to file', 10, 0
    write_to_file_error_len: equ  $ -  write_to_file_error
    
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

%define COUNTER dword [ebp-158] ;counter: count words in case 1, count appearances of input string in case 2
%define CASE_NUMBER byte [ebp-159] ;case 0: task2a, case 1: '-w', case 2: '-ws'
%define IS_LAST_CHAR_SPACE dword [ebp-163] ;flag that says: 0: last char is space, 1: last char isn't space
main:
	enter 163, 0 ;reserving 150 bytes for buffer, 4 bytes for file descriptor, 4 bytes for COUNTER, 1 bytes for CASE_NUMBER, 4 bytes for IS_LAST_CHAR_SPACE
	mov COUNTER, 0 ;set counter to 0
	mov CASE_NUMBER, 0 ;set default case as case 0
	mov IS_LAST_CHAR_SPACE, 0 ;set default as 0 since we are in the begining of the file
	mov ebx, [ebp+12] ;argv
	add ebx, 4 ;ebx is now argv[1]

case_check:
	push dword [ebx] ;set char *str2 for function cmpstr
	push w_opt ;set char *str1 for function cmpstr
	
	call cmpstr
	
	pop edx ;stack maintanance
	pop edx ;stack maintanance
	cmp eax, 0 ;check if argv[1] equals '-w'
	je w_case
	
	push dword [ebx] ;set char *str2 for function cmpstr
	push ws_opt ;set char *str1 for function cmpstr
	
	call cmpstr
	
	pop edx ;stack maintanance
	pop edx ;stack maintanance
	cmp eax, 0 ;check if argv[1] equals '-ws'
	je ws_case
	jmp safe_open ;argv[1] is the file name

w_case:
	mov CASE_NUMBER, 1 ;set case 1
	add ebx, 4 ;the file name is argv[2]
	jmp safe_open
	
ws_case:
	mov CASE_NUMBER, 2 ;set case 2
	add ebx, 4 ;the word for case '-ws' is argv[2]
	push dword cmp_str ;set char *str2 for function load_string_to_buffer
	push dword [ebx] ;set char *str1 for function load_string_to_buffer
	
	call load_string_to_buffer
	
	pop edx ;stack maintanance
	pop edx ;stack maintanance
	add ebx, 4 ;the file name is argv[3]
	jmp safe_open 

safe_open:
	mov ebx, [ebx] ;retrieves path of file
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

	call clean_buffer 
	
	mov ebx, ebp
	sub ebx, 150 
	push ebx ;set char *buf for function read
	push dword [ebp-154] ;set int fd for function read
	
	call read
	
	pop ebx ;stack maintanance
	pop ebx ;stack maintanance
	pop ebx ;stack maintanance
	cmp eax, 0 ;buffer not empty check
	jg move_to_correct_option
	jmp finish_reading

clean_buffer:
    mov dl, [ebx] ;read next char in the buffer
    test dl, dl ;check if \0
    jne .rem_byte
    ret

.rem_byte:
    mov [ebx], byte 0 ;remove one byte
    inc ebx ;move the buffer pointer
    jmp clean_buffer

move_to_correct_option:
	mov bl, CASE_NUMBER ;get case number
	cmp bl, 0 ;check if case 0
	je print
	cmp bl, 1 ;check if case 1
	je count_words_preperation
	cmp bl, 2 ;check if case 2
	je count_words_string_preperation
	
finish_reading:
	push dword [ebp-154] ;set int fd for function close
	
	call close
	
	pop ebx ;stack maintanance
	mov bl, CASE_NUMBER 
	cmp bl, 1 ;check if case 1
	je count_words_printer 
	cmp bl, 2 ;check if case 2
	je count_words_string_printer
	jmp return ;case 0
	
count_words_printer:
	push dword count_words_str_len ;set int size for function write
    push dword count_words_str ;set char *buf for function write
    push dword 1 ;set int fd for function write (stdout)
    
    call write

    pop ebx ;stack maintanance
    pop ebx ;stack maintanance
    pop ebx ;stack maintanance
	mov ebx, COUNTER 
	mov eax, int_to_str_buffer
	push eax ;set int number for function utoa_s
	push ebx ;set char *buf for function utoa_s
	
	call utoa_s
	
	pop ebx ;stack maintanance
	pop ebx ;stack maintanance
	push dword 50 ;set int size for function write
	push dword int_to_str_buffer ;set char *buf for function write
	push dword 1 ;set int fd for function write (stdout)
	
	call write
	
	pop ebx ;stack maintanance
	pop ebx ;stack maintanance
	pop ebx ;stack maintanance
	push dword 1 ;set int size for function write
    push dword end_line ;set char *buf for function write
    push dword 1 ;set int fd for function write (stdout)
    
    call write

    pop ebx ;stack maintanance
    pop ebx ;stack maintanance
    pop ebx ;stack maintanance
	jmp return
	
count_words_string_printer:
	push dword count_words_string_str_1_len ;set int size for function write
    push dword count_words_string_str_1 ;set char *buf for function write
    push dword 1 ;set int fd for function write (stdout)
    
    call write

    pop ebx ;stack maintanance
    pop ebx ;stack maintanance
    pop ebx ;stack maintanance
    push dword 50 ;set int size for function write
    push dword cmp_str ;set char *buf for function write
    push dword 1 ;set int fd for function write (stdout)
    
    call write

    pop ebx ;stack maintanance
    pop ebx ;stack maintanance
    pop ebx ;stack maintanance
    push dword count_words_string_str_2_len ;set int size for function write
    push dword count_words_string_str_2 ;set char *buf for function write
    push dword 1 ;set int fd for function write (stdout)
    
    call write

    pop ebx ;stack maintanance
    pop ebx ;stack maintanance
    pop ebx ;stack maintanance
	mov ebx, COUNTER
	mov eax, int_to_str_buffer
	push eax ;set int number for function utoa_s
	push ebx ;set char *buf for function utoa_s
	
	call utoa_s
	
	pop ebx ;stack maintanance
	pop ebx ;stack maintanance
	push dword 50 ;set int size for function write
	push dword int_to_str_buffer ;set char *buf for function write
	push dword 1 ;set int fd for function write (stdout)
	
	call write
	
	pop ebx ;stack maintanance
	pop ebx ;stack maintanance
	pop ebx ;stack maintanance
	push dword 1 ;set int size for function write
    push dword end_line ;set char *buf for function write
    push dword 1 ;set int fd for function write (stdout)
    
    call write

    pop ebx ;stack maintanance
    pop ebx ;stack maintanance
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


count_words_preperation:
	mov ebx, ebp
	sub ebx, 163
	push ebx ;set is_last_char_sapce 
	push eax ;set size
	mov ebx, ebp
	sub ebx, 150 ;set pointer to buffer start
	push ebx
	mov ebx, ebp
	sub ebx, 158
	push ebx ;set counter
	
	call count_words
	
	pop ebx ;stack maintanance
	pop ebx ;stack maintanance
	pop ebx ;stack maintanance
	pop ebx ;stack maintanance
	jmp buffer_read	

count_words:
	enter 0, 0
	mov edx, dword [ebp+20] ;set is_last_char_sapce 
	mov ecx, dword [ebp+16] ;get size
	mov ebx, dword [ebp+12] ;get pointer to buffer start
	
	sub ebx, 1 ;reset before loop starting
	add ecx, 1 ;reset before loop starting
	
loop_over_chars:
	call find_next_char
	
    inc ebx ;move to next char
    loop loop_over_chars ;loop until size is 0
    jmp finish_count_words

find_next_char:
	inc ebx
	dec ecx
	cmp ecx, 0
	je finish_count_words ;reached end of text
	cmp [ebx], byte 9 ;check horizontal tab
    je set_last_char_was_space
	cmp [ebx], byte 10 ;check new line
    je set_last_char_was_space
    cmp [ebx], byte 32 ;check space
    je set_last_char_was_space
    jmp is_new_word_found ;the next char isn't space, so we found a start of new word

set_last_char_was_space:
	mov [edx], dword 0 ;set is_last_char_sapce to 0
    jmp find_next_char

is_new_word_found:
	cmp [edx], dword 0 ;check if last char was space
	jne move_to_next_word ;we arn't in a start of a new string (buffer split issue)
	mov eax, [ebp+8] ;get counter
    inc dword [eax] ;increase counter
    jmp move_to_next_word
	
move_to_next_word:
	mov [edx], dword 1 ;last char isn't space anymore
	inc ebx
	dec ecx
	cmp ecx, 0
	je finish_count_words ;reached end of text
	cmp [ebx], byte 9 ;check horizontal tab
    je set_last_char_was_space
	cmp [ebx], byte 10 ;check new line
	je set_last_char_was_space
	cmp [ebx], byte 32 ;check space
	je set_last_char_was_space
	jmp move_to_next_word ;the next char isn't space, so we are still in the same word
	
finish_count_words:
	leave ;returning reserved bytes
	ret
	
count_words_string_preperation:
	mov ebx, ebp
	sub ebx, 163
	push ebx ;set is_last_char_sapce 
	push eax ;set size
	mov ebx, ebp
	sub ebx, 150 ;set pointer to buffer start
	push ebx
	mov ebx, ebp
	sub ebx, 158
	push ebx ;set counter
	
	call count_words_string
	
	pop ebx ;stack maintanance
	pop ebx ;stack maintanance
	pop ebx ;stack maintanance
	jmp buffer_read	

count_words_string:
	enter 0, 0	
	mov edx, dword [ebp+20] ;set is_last_char_sapce
	mov ecx, dword [ebp+16] ;get size
	mov ebx, dword [ebp+12] ;get pointer to buffer start
	
	sub ebx, 1 ;reset before loop starting
	add ecx, 1 ;reset before loop starting
	
loop_over_chars_ws_case:
	call find_next_char_ws_case
	
    inc ebx ;move to next char
    loop loop_over_chars_ws_case ;loop until size is 0
    jmp finish_count_words_string

find_next_char_ws_case:
	inc ebx
	dec ecx
	cmp ecx, 0
	je finish_count_words_string ;reached end of text
	cmp [ebx], byte 9 ;check horizontal tab
    je set_last_char_was_space_ws_case
	cmp [ebx], byte 10 ;check new line
    je set_last_char_was_space_ws_case
    cmp [ebx], byte 32 ;check space
    je set_last_char_was_space_ws_case
    jmp is_new_word_found_ws_case ;the next char isn't space, so we found a start of new word

set_last_char_was_space_ws_case:
	mov [edx], dword 0 ;set is_last_char_sapce to 0
    jmp find_next_char_ws_case

is_new_word_found_ws_case:
	cmp [edx], dword 0 ;check if last char was space
	jne move_to_next_word_ws_case ;we arn't in a start of a new string (buffer split issue)
	
	call check_string
	
    jmp move_to_next_word_ws_case
	
move_to_next_word_ws_case:
	mov [edx], dword 1 ;last char isn't space anymore
	inc ebx
	dec ecx
	cmp ecx, 0
	je finish_count_words_string ;reached end of text
	cmp [ebx], byte 9 ;check horizontal tab
    je set_last_char_was_space_ws_case
	cmp [ebx], byte 10 ;check new line
	je set_last_char_was_space_ws_case
	cmp [ebx], byte 32 ;check space
	je set_last_char_was_space_ws_case
	jmp move_to_next_word_ws_case ;the next char isn't space, so we are still in the same word
	
check_string:
	push ebx ;set char *str2 for function cmpstr_in_text
	push dword cmp_str ;set char *str1 for function cmpstr_in_text
	
	call cmpstr_in_text
	
	pop ebx ;stack maintanance
	pop ebx ;stack maintanance
    
	cmp eax, 0 ;check if argv[2] was found
	je string_found
    ret

string_found:
	mov eax, [ebp+8] ;get counter
    inc dword [eax] ;increase counter
	ret
	
finish_count_words_string:
	leave ;returning reserved bytes
	ret

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
	cmp eax, -1
    jle read_failed
	jmp FINISH

read_failed:
	push dword read_from_file_error_len ;set int size for function write
    push dword read_from_file_error ;set char *buf for function write
    push 1 ;set int fd for function write (stdout)
    
    call write

    pop ebx ;stack maintanance
    pop ebx ;stack maintanance
    pop ebx ;stack maintanance
    jmp handle_error

write:
	push ebp
	push ebx
	mov	ebp, esp
	mov eax, 4 ;Sys_write system call number
	mov ebx, [ebp+12] ;retrieves the first function argument
	mov ecx, [ebp+16] ;retrieves the second function argument
	mov edx, [ebp+20] ;retrieves the third function argument
	int 0x80
	cmp eax, -1
    jle write_failed
	jmp FINISH
	
write_failed:
	push dword write_to_file_error_len ;set int size for function write
    push dword write_to_file_error ;set char *buf for function write
    push 1 ;set int fd for function write (stdout)
    
    call write

    pop ebx ;stack maintanance
    pop ebx ;stack maintanance
    pop ebx ;stack maintanance
    jmp handle_error

open:
	push ebp
	push ebx
	mov	ebp, esp
	mov eax, 5 ;Sys_open system call number
	mov ebx, [ebp+12] ;retrieves the first function argument
	mov ecx, [ebp+16] ;retrieves the second function argument
	mov edx, 0777 ;read, write and execute by all
	int 0x80
	cmp eax, -1
    jle open_failed
	jmp FINISH
	
open_failed:
	push dword open_file_error_len ;set int size for function write
    push dword open_file_error ;set char *buf for function write
    push 1 ;set int fd for function write (stdout)
    
    call write

    pop ebx ;stack maintanance
    pop ebx ;stack maintanance
    pop ebx ;stack maintanance
    jmp handle_error

close:
	push ebp
	push ebx
	mov	ebp, esp
	mov eax, 6 ;Sys_close system call number
	mov ebx, [ebp+12] ;retrieves the first function argument
	int 0x80
	cmp eax, -1
    jle close_failed
	jmp FINISH
	
close_failed:
	push dword close_file_error_len ;set int size for function write
    push dword close_file_error ;set char *buf for function write
    push 1 ;set int fd for function write (stdout)
    
    call write

    pop ebx ;stack maintanance
    pop ebx ;stack maintanance
    pop ebx ;stack maintanance
    jmp handle_error

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
	
cmpstr:
	push ebp
	push ebx
	mov	ebp, esp
	mov	eax,-1 ;set eax as the index from where to start reading

.L4:
	add eax, 1 ;increase the value of eax
	mov ebx, eax ;set ebx offset from the start of string1 before the reading
	mov ecx, eax ;set ebx offset from the start of string2 before the reading
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
	jmp .L4 ;loop back if no difference found
	
.compare:
	mov eax, ebx ;set eax to get the current char where the loop ended
	sub eax, ecx ;compare between the last chars that were read before the loop ended
	jmp FINISH ;jump to the end of the program
	
load_string_to_buffer:
    push ebp
	push ebx
	mov	ebp, esp
    mov eax, [ebp+12] ;get argument string
    mov ebx, [ebp+16] ;get argument buffer
    mov [ebp-8], ebx ;save buffer address
    mov ebx, [ebp-8] ;restore buffer pointer to start

.load_char:
	movzx ecx, byte [eax]
    mov [ebx], cl ;write char to buffer
    inc eax ;move string pointer to the next cell
    inc ebx ;move buffer pointer to the next cell
    test cl, cl ;end of string
    je .finish_convert_string
    jmp .load_char

.finish_convert_string:
    mov [ebx], byte 0 ;close the str with an \0
    jmp FINISH
    
cmpstr_in_text:
	push ebp
	push ebx
	mov	ebp, esp
	mov	eax,-1 ;set eax as the index from where to start reading

.L5:
	add eax, 1 ;increase the value of eax
	mov ebx, eax ;set ebx offset from the start of string1 before the reading
	mov ecx, eax ;set ebx offset from the start of string2 before the reading
	add ebx, [ebp+12] ;get the next char in string1
	add ecx, [ebp+16] ;get the next char in string2
	movzx ebx, BYTE [ebx] ;zero extends the value of ebx
	movzx ecx, BYTE [ecx] ;zero extends the value of ecx
	test bl,bl ;check if bl equals to the zero char (we found the string in the test)
	je .check_if_word ;end of string1 found
	test cl,cl ;check if cl equals to the zero char (we reached the end of file without finding the string)
	je .string_not_found_in_text ;end of string2 found
	cmp ebx, ecx ;compare between char1 to char2 of the strings
	jne .string_not_found_in_text ;different chars were found (we found differences between the chars)
	jmp .L5 ;loop back if no difference found

.check_if_word:
	cmp ecx, byte 9 ;check horizontal tab
    je .string_found_in_text
	cmp ecx, byte 10 ;check new line
	je .string_found_in_text
	cmp ecx, byte 32 ;check space
	je .string_found_in_text
	jmp .string_not_found_in_text ;the word is a substring of another word

.string_found_in_text:
	mov eax, dword 0 ;string was found
	jmp FINISH ;jump to the end of the program


.string_not_found_in_text:
	mov eax, dword -1 ;string wasn't found
	jmp FINISH ;jump to the end of the program

FINISH:
	mov esp, ebp
	pop ebx
	pop ebp
	ret
