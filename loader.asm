extern entry

section .data:
	wlc db "Loaded...", 10
	sz equ $-wlc

section .text:
	global _start

_start:
	mov rax, 1
	mov rdi, 1
	lea rsi, wlc
	mov rdx, sz
	syscall

	call entry

	mov rax, 60
	mov rdi, 0
	syscall
