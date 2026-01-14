section .bss
	; i think i forgot already lmao
	buffer resb 100

section .data
	wlc db "Loading...", 10
	len equ $ - wlc

	prompt db "> ", 0

	user_typed db "Input: ", 10
	user_typed_len equ $ - user_typed

section .text
	global _start

cli_loop:
	xor r9, r9

	.loop:
		; Output 'PS1' Prompt
		mov rax, 1
		mov rdi, 1
		lea rsi, [prompt]
		mov rdx, 3
		syscall

		; Get Input
		mov rax, 0
		mov rdi, 0
		lea rsi, [buffer]
		mov rdx, 255
		syscall

		cmp rax, 0
		je .loop

		mov rax, 1
		mov rdi, 1
		lea rsi, [user_typed]
		mov rdx, user_typed_len
		syscall

		mov rax, 1
		mov rdi, 1
		lea rsi, [buffer]
		mov rdx, 255
		syscall

		jmp .loop

	exit_loop:
		ret

_start:
	call cli_loop
