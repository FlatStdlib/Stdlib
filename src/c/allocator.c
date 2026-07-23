#include "../../headers/fsl.h"

#define _HEAP_PAGE_SZ_  4096
int _HEAP_MULTIPLER_    = 2;
int _HEAP_PAGE_         = _HEAP_PAGE_SZ_ * 2;
heap_t _HEAP_ 			= NULL;
int used_mem 			= 0;
int HEAP_DEBUG 			= 0;
const int HEAP_META_SZ 	= sizeof(__meta__);

#if defined(_WIN32) ||  defined(_WIN64)
char _TEST_HEAP_[8192];
#endif

public int __get_total_mem_used__(void) 
{ return used_mem; }

public int __is_heap_init__(void)
{ return (_HEAP_ ? 1 : 0); }

public fn set_heap_sz(int n)
{ _HEAP_PAGE_ = n; }

public fn set_heap_debug(void)
{ HEAP_DEBUG = 1; }

public fn init_mem(void) {
    long ret;
    #ifdef __i386__
		register long syscall asm(__EAX__) = _SYS_MMAP_PGOFF;
		register long arg1 asm(__EBX__) = 0;
		register long arg2 asm(__ECX__) = _HEAP_PAGE_;
		register long arg3 asm(__EDX__) = 0x1|0x2;
		register long arg4 asm(__ESI__) = 0x2|0x20;
		register long arg5 asm(__EDI__) = -1;
		register long arg6 asm(__EBP__) = 0;
		asm(EXECUTE_SYSCALL);
        register long check asm(__EAX__);
        ret = check;
    #elif defined(__x86_64__)
        ret = __sys_mmap(0, _HEAP_PAGE_, 0x1|0x2, 0x2|0x20, -1, 0);
        if (ret <= 0)
            fsl_panic("mmap failed!");
    #endif


    #if defined(_WIN32) || defined(_WIN64)
        _HEAP_ = (heap_t)_TEST_HEAP_;
    #else
        _HEAP_ = (heap_t)ret;
    #endif

    // Clear the heap to mark all memory as free
    mem_set(_HEAP_, 1, _HEAP_PAGE_);

    if (HEAP_DEBUG || __FSL_DEBUG__)
        print("[ALLOCATOR]: Heap initialized!\n");
}

private int find_space(int space)
{
    for (int i = 0; i <= _HEAP_PAGE_ - space; i++) {
        int free = 1;
        for (int j = 0; j < space; j++) {
            if (((char *)_HEAP_)[i + j] != 1) {
                free = 0;
                break;
            }
        }
        if (free)
            return i;
    }
    return -1;
}

public any allocate(int sz, int len) {
    if (!len) return NULL;

    int space_left = _HEAP_PAGE_ - used_mem;
    int mem_needed = (sz ? sz * len : len) + HEAP_META_SZ;

    if (space_left < mem_needed)
        return NULL;

    int spot = find_space(mem_needed);
    if (spot == -1)
        fsl_panic("Unable to find space!\n");

    any ptr = (char *)_HEAP_ + spot;
    __meta__ c;
    c.size = sz;
    c.length = len;
    c.id = 0x7C;

    mem_cpy(ptr, &c, HEAP_META_SZ);
    mem_set(ptr + HEAP_META_SZ, 0, mem_needed - HEAP_META_SZ);

    used_mem += mem_needed;

	if(HEAP_DEBUG || __FSL_DEBUG__)
	{
		char buff[100];
		ptr_to_str(ptr, buff);
		print("[ALLOCATOR]: Allocated "), _printi(sz ? sz * len : len), print(" to "), println(buff);
        int n = sz ? sz * len : len;
	}

    return (any)((char *)ptr + HEAP_META_SZ);
}

public any reallocate(any p, int sz)
{
    any new_p = allocate(0, sz + 1);
    if(!new_p)
        fsl_panic("Segfault");
        
    mem_cpy(new_p, p, __get_size__(p));
    pfree(p, 1);

    return new_p;
}

public __meta__ *__get_meta__(any ptr)
{
	return ((__meta__ *)((char *)ptr - HEAP_META_SZ));
}

public int __get_size__(any ptr)
{
	__meta__ *info = __get_meta__(ptr);
	return !info->size ? info->length : info->size * info->length;
}

public int __get_array_size__(any ptr)
{ return __get_meta__(ptr)->length; }

public fn pfree_array(array p)
{
    if(!p)
        return;

    __meta__ *m = __get_meta__(p);
    if(m->id != 0x7C)
        fsl_panic("Invalid heap pointer!");

	for(int i = 0; p[i] != NULL; i++)
        if(p[i])
		    pfree(p[i], 1);
}

public fn _pfree(any ptr) { pfree(ptr, 1); }
public fn pfree(any ptr, int clean)
{
    if (!ptr) return;

    __meta__ *m = __get_meta__(ptr);
    if(ptr < _HEAP_ && ptr >= (_HEAP_ + _HEAP_PAGE_))
        fsl_panic("Invalid heap pointer!");

    int payload = m->size ? m->size * m->length : m->length;
    int total   = payload + HEAP_META_SZ;

	if(clean)
    	mem_set(m, 1, total);

    used_mem -= total;

	if(HEAP_DEBUG || __FSL_DEBUG__)
	{
    	char buff[100];
    	ptr_to_str(m, buff);
    	print("[ALLOCATOR]: Freeing "), printi(payload), print(" - memory block @ "), println(buff);
    }
}

#if !defined(_WIN32) && !defined(_WIN64)
public fn uninit_mem(void)
{
	if(HEAP_DEBUG || __FSL_DEBUG__) {
		println("[ALLOCATOR]: Uninitializing");

        // #ifdef __MEM_LEAK_FINDER__
        ((string)_HEAP_)[_HEAP_PAGE_ - 1] = '\0';
        string start = (string)_HEAP_;
        string last = (((string)_HEAP_) + _HEAP_PAGE_);
        int found = 0;
        for(int i = 0; start < last; i++, start++)
        {

            if(start[0] == 0x7C)
            {
                if(found > 0)
                    println("================");

                found = 1;
                println("Memory block leak found!");
                __meta__ *info = (__meta__ *)((char *)start - (sizeof(int) + sizeof(size_t)));

                string last = (start + sizeof(int) + (info->length - 1));
                int sz = __get_size__(start + sizeof(int));
                char ptr[100];
                ptr_to_str(info, ptr);

                if(last[0] == '\0')
                    print("Pointer: "), print(ptr), print("Size: "), printi(sz), print(" | Data: "), println(start + sizeof(int));
            }
        }
        // #endif
    }
        
	__syscall__((long)_HEAP_, _HEAP_PAGE_, 0, 0, 0, 0, _SYS_MUNMAP);
}
#endif
