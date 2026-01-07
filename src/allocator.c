#include "../headers/clibp.h"

int _HEAP_PAGE_         = 4096;
int _HEAP_PAGE_SZ_ 		= 4096;
heap_t _HEAP_ 			= NULL;
int used_mem 			= 0;
int HEAP_DEBUG 			= 0;
const int HEAP_META_SZ 	= sizeof(__meta__);

int __get_total_mem_used__(void) { return used_mem; }
int __is_heap_init__() { return (_HEAP_ ? 1 : 0); }

fn set_heap_sz(int n)
{
	_HEAP_PAGE_ = n;
}

fn set_heap_debug()
{
	HEAP_DEBUG = 1;
}

fn req_memory()
{
	if(!_HEAP_PAGE_) _HEAP_PAGE_ = _HEAP_PAGE_SZ_;
    if(__syscall__((long)_HEAP_, _HEAP_PAGE_, 0x1 | 0x2, 0, 0, 0, _SYS_MPROTECT) != 0)
    {
        println("Segfault");
        return;
    }
}

fn init_mem() {
	_printi(_HEAP_PAGE_), print("\n");
    long ret = __sys_mmap(0, _HEAP_PAGE_, 0x1|0x2, 0x2|0x20, -1, 0);
    if (ret <= 0) {
        println("[ - ] Error, mmap failed:"), _printi(ret), print("\n");
        return;
    }

	_HEAP_ = (ptr)ret;

    // Clear the heap to mark all memory as free
    mem_set(_HEAP_, 1, _HEAP_PAGE_);

    if (HEAP_DEBUG)
        print("[ + ] Heap initialized!\n");
}

static int find_space(int space)
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

any allocate(int sz, int len) {
    if (!len) return NULL;

    int space_left = _HEAP_PAGE_ - used_mem;
    int mem_needed = (sz ? sz * len : len) + HEAP_META_SZ;

    if (space_left < mem_needed)
        return NULL;

    int spot = find_space(mem_needed);
    if (spot == -1) {
        print("[ - ] Unable to find space!\n");
        return NULL;
    }

    any ptr = (char *)_HEAP_ + spot;
    __meta__ c = { .size = sz, .length = len, .id = 0x7C };

    mem_cpy(ptr, &c, HEAP_META_SZ);
    mem_set(ptr + HEAP_META_SZ, 0, mem_needed - HEAP_META_SZ);

    used_mem += mem_needed;

	if(HEAP_DEBUG)
	{
		char buff[100];
		ptr_to_str(ptr, buff);
		print("[ + ] Allocated "), _printi(sz ? sz * len : len), print(" to "), println(buff);
	}

    return (any)((char *)ptr + HEAP_META_SZ);
}

__meta__ *__get_meta__(any ptr)
{
	return ((__meta__ *)((char *)ptr - HEAP_META_SZ));
}

int __get_size__(any ptr)
{
	__meta__ *info = __get_meta__(ptr);
	return !info->size ? info->length : info->size * info->length;
}

fn pfree(any ptr)
{
    if (!ptr) return;

    __meta__ *m = __get_meta__(ptr);

    int payload = m->size ? m->size * m->length : m->length;
    int total   = payload + HEAP_META_SZ;

    mem_set(m, 1, total);
    used_mem -= total;
}

fn uninit_mem()
{
	__syscall__((long)_HEAP_, _HEAP_PAGE_, 0, 0, 0, 0, _SYS_MUNMAP);
}
