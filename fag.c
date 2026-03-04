
/* Following ABI Standards Here, A Universial user-space syscall */
// long __syscall__(register long arg1, register long arg2, register long arg3, register long arg4, register long arg5, register long arg6, register long sys)
// {
//     long ret;
//     __asm__ volatile (
//         "push %[a6]\n\t"
//         "mov %[a1], %%ebx\n\t"
//         "mov %[a2], %%ecx\n\t"
//         "mov %[a3], %%edx\n\t"
//         "mov %[a4], %%esi\n\t"
//         "mov %[a5], %%edi\n\t"
//         "mov $192, %%eax\n\t"   /* sys_mmap */
//         "int $0x80\n\t"
//         "add $4, %%esp\n\t"     /* pop a6 back */
//         : "=a" (ret)
//         : [a1]"r"(arg1), [a2]"r"(arg2), [a3]"r"(arg3),
//           [a4]"r"(arg4), [a5]"r"(arg5), [a6]"r"(arg6)
//         : "ebx","ecx","edx","esi","edi","memory"
//     );
//     return ret;
// }

long __syscall__(long a1, long a2, long a3, long a4, long a5, long a6) {
    long ret;

    __asm__ volatile (
        "movl $4, %%eax\n\t"
    );

    return ret;
}

void start()
{
    long ret = __syscall__(0, 4096, 0x1|0x2, 0x2|0x20, -1, 0);
    // if (ret <= 0)
    //     __syscall__(1, (long)"mmap failed!", 25, 0, 0, 0, 4);


    // __syscall__(1, (long)"[ + ] Heap initialized!\n", 25, 0, 0, 0, 4);
    // __syscall__(0, 0, 0, 0, 0, 0, 1);
}