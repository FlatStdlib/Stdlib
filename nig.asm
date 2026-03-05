global test_fnc

section .bss
    global buffer
    buffer resd 1024

section .text

test_fnc:
    mov eax, 4
    mov ebx, 1
    mov ecx, buffer
    mov edx, 100
    int 0x80
    ret