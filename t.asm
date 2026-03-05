section .bss
    buffer resd 8
    nptr resd 1

section .data
    success db 10, "Worked", 10, 0
    len equ $ - success

    mmap_args:
    dd 0
    dd 4096
    dd 3
    dd 0x22
    dd -1
    dd 0

section .text
    global _start

_start:
    mov eax, 90
    mov ebx, mmap_args
    int 0x80

    mov [nptr], eax

    cmp eax, -4095
    jae .failed_exit

    mov ebx, [nptr]
    mov byte [ebx + 0], 'h'
    mov byte [ebx + 1], 'i'
    mov byte [ebx + 2], 0

    mov eax, 4
    mov ebx, 1
    mov ecx, [nptr]
    mov edx, 3
    int 0x80

    mov eax, 4
    mov ebx, 1
    lea ecx, success
    mov edx, len
    int 0x80

    mov eax, 1
    xor ebx, ebx
    int 0x80

    .failed_exit:
        mov eax, 1
        mov ebx, 1
        int 0x80
