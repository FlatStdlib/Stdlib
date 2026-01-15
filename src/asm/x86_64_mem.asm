global x86_64_mem_set
glball x86_64_mem_cpy

section .bss
	buffer resb 1024
	buff resb 1024

section .data

section .text
	global _start

; @label: strcpy
; @arguments:
;	RDI = char *
;	AL = char
; 	RDX = size
;
; @return: void
x86_64_mem_set: 		; For external use in C
__mem_set:
	xor r10, r10

	.mem_set_loop:
		cmp rdx, r10
		je .exit_mem_set

		mov byte [rdi + r10], al
		inc r10

		jmp .mem_set_loop

	.exit_mem_set:
		ret

; @label: mem_cpy
; @arguments:
; 	RDI = char *
; 	RSI = char *
; 	RDX = size
;
; @return(RAX): idx as success or -1 as mismatch in
x86_64_mem_cpy:
__mem_cpy:
	xor r10, r10

	.mem_cpy_loop:
		cmp rdx, r10
		je .exit_success_mem_cpy

		mov al, byte [rsi + r10]
		mov byte [rdi + r10], al

		inc r10
		jmp .mem_cpy_loop

	.exit_success_mem_cpy:
		mov rax, rdx
		ret
