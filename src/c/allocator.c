#include "../../headers/fsl.h"

#define _HEAP_PAGE_SZ_  4096
int _HEAP_MULTIPLER_    = 2;
int _HEAP_PAGE_         = _HEAP_PAGE_SZ_ * 2;
heap_t _HEAP_ 			= NULL;
int used_mem 			= 0;
int HEAP_DEBUG 			= 0;
const int HEAP_META_SZ 	= sizeof(__meta__);

public int __get_total_mem_used__(void) 
{ return used_mem; }

public int __is_heap_init__(void)
{ return (_HEAP_ ? 1 : 0); }

public fn set_heap_sz(int n)
{ _HEAP_PAGE_ = n; }

public fn set_heap_debug(void)
{ HEAP_DEBUG = 1; }

public fn init_mem(void) {
    long ret = __sys_mmap(0, _HEAP_PAGE_, 0x1|0x2, 0x2|0x20, -1, 0);
    if (ret <= 0)
        fsl_panic("mmap failed!");

	_HEAP_ = (ptr)ret;

    // Clear the heap to mark all memory as free
    mem_set(_HEAP_, 1, _HEAP_PAGE_);

    if (HEAP_DEBUG || __FSL_DEBUG__)
        print("[ + ] Heap initialized!\n");
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
    __meta__ c = { .size = sz, .length = len, .id = 0x7C };

    mem_cpy(ptr, &c, HEAP_META_SZ);
    mem_set(ptr + HEAP_META_SZ, 0, mem_needed - HEAP_META_SZ);

    used_mem += mem_needed;

	if(HEAP_DEBUG || __FSL_DEBUG__)
	{
		char buff[100];
		ptr_to_str(ptr, buff);
		print("[ + ] Allocated "), _printi(sz ? sz * len : len), print(" to "), println(buff);
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

public fn pfree_array(array p)
{
	for(int i = 0; p[i] != NULL; i++)
		pfree(p[i], 1);
}

public fn _pfree(any ptr) { pfree(ptr, 1); }
public fn pfree(any ptr, int clean)
{
    if (!ptr) return;

    __meta__ *m = __get_meta__(ptr);

    int payload = m->size ? m->size * m->length : m->length;
    int total   = payload + HEAP_META_SZ;

	if(clean)
    	mem_set(m, 1, total);

    used_mem -= total;

	if(HEAP_DEBUG || __FSL_DEBUG__)
	{
    	char buff[100];
    	ptr_to_str(m, buff);
    	print("[ + ] Freeing memory block @ "), println(buff);
    }
}

public fn uninit_mem(void)
{
	if(HEAP_DEBUG || __FSL_DEBUG__)
		println("[ + ] Uninitializing");
        
	__syscall__((long)_HEAP_, _HEAP_PAGE_, 0, 0, 0, 0, _SYS_MUNMAP);
}
