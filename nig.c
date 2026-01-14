#include <clibp.h>
/* elf_parser.c — zero libc ELF64 parser */

typedef long           s64;

/* ---- minimal syscalls (Linux x86_64) ---- */
/* ---- tiny helpers ---- */
static void puthex(u64 v) {
    char buf[17]; int i=16; buf[i]=0;
    const char *h="0123456789abcdef";
    while(i--) { buf[i]=h[v&0xf]; v>>=4; }
    print_sz(buf, 16);
}

/* ---- ELF structures ---- */

#define EI_NIDENT 16

typedef struct {
    u8  e_ident[EI_NIDENT];
    u16 e_type;
    u16 e_machine;
    u32 e_version;
    u64 e_entry;
    u64 e_phoff;
    u64 e_shoff;
    u32 e_flags;
    u16 e_ehsize;
    u16 e_phentsize;
    u16 e_phnum;
    u16 e_shentsize;
    u16 e_shnum;
    u16 e_shstrndx;
} Elf64_Ehdr;

typedef struct {
    u32 p_type;
    u32 p_flags;
    u64 p_offset;
    u64 p_vaddr;
    u64 p_paddr;
    u64 p_filesz;
    u64 p_memsz;
    u64 p_align;
} Elf64_Phdr;

typedef struct {
    u32 sh_name;
    u32 sh_type;
    u64 sh_flags;
    u64 sh_addr;
    u64 sh_offset;
    u64 sh_size;
    u32 sh_link;
    u32 sh_info;
    u64 sh_addralign;
    u64 sh_entsize;
} Elf64_Shdr;

/* ---- entry ---- */
int entry(void) {
    static u8 file[1024*1024];

    int fd = open_file("a.out", 0, 0);
    if(fd < 0) {
        print("open failed\n");
        __exit(1);
    }

    s64 sz = __syscall__(fd, (long)file, sizeof(file), 0, 0, 0, _SYS_WRITE);
    if(sz < sizeof(Elf64_Ehdr)) {
        print("file too small\n");
        __exit(1);
    }

    Elf64_Ehdr *eh = (Elf64_Ehdr*)file;

    /* ELF magic */
    if(eh->e_ident[0]!=0x7f ||
       eh->e_ident[1]!='E' ||
       eh->e_ident[2]!='L' ||
       eh->e_ident[3]!='F') {
        print("not an ELF\n");
        __exit(1);
    }

    print("ELF detected\n");
    print("Entry: 0x"); puthex(eh->e_entry); print("\n");

    print("Program headers:\n");
    Elf64_Phdr *ph = (Elf64_Phdr*)(file + eh->e_phoff);
    for(u16 i=0;i<eh->e_phnum;i++) {
        print("  PH "); puthex(i);
        print(" type "); puthex(ph[i].p_type);
        print(" vaddr "); puthex(ph[i].p_vaddr);
        print(" filesz "); puthex(ph[i].p_filesz);
        print("\n");
    }

    print("Section headers:\n");
    Elf64_Shdr *sh = (Elf64_Shdr*)(file + eh->e_shoff);
    const char *shstr = (const char*)(file + sh[eh->e_shstrndx].sh_offset);

    for(u16 i=0;i<eh->e_shnum;i++) {
        const char *name = shstr + sh[i].sh_name;
        print("  SH "); puthex(i);
        print(" "); print((string)name);
        print(" addr "); puthex(sh[i].sh_addr);
        print(" size "); puthex(sh[i].sh_size);
        print("\n");
    }

    print("Done.\n");
    __exit(0);
}
