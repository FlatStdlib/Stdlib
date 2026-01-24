#include "../../headers/clibp.h"

#define _HEAP_PAGE_SZ_  4096
int _HEAP_MULTIPLER_    = 2;
int _HEAP_PAGE_         = _HEAP_PAGE_SZ_ * 2;
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
    /* Save old heap and free it */
    i32 old_size = _HEAP_PAGE_;
    heap_t old_heap = _HEAP_;
    _HEAP_ = NULL;

    /* Set new heap size to increase */
    _HEAP_MULTIPLER_++;
    _HEAP_PAGE_ = _HEAP_PAGE_SZ_ * _HEAP_MULTIPLER_;

    /* Initialize a new heap */
    init_mem();
    mem_cmp(_HEAP_, old_heap, old_size);

    /* Save new heap to free the old one */
    i32 new_size = _HEAP_PAGE_;
    heap_t new_heap = _HEAP_;

    _HEAP_ = old_heap;
    _HEAP_PAGE_ = old_size;
    uninit_mem();

    _HEAP_ = new_heap;
    _HEAP_PAGE_ = new_size;
}

fn init_mem() {
    long ret = __sys_mmap(0, _HEAP_PAGE_, 0x1|0x2, 0x2|0x20, -1, 0);
    if (ret <= 0)
        clibp_panic("mmap failed!");

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
    if (spot == -1)
        clibp_panic("Unable to find space!\n");

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

any reallocate(any p, int sz)
{
    any new_p = allocate(0, sz + 1);
    if(!new_p)
        clibp_panic("Segfault");
        
    mem_cpy(new_p, p, __get_size__(p));
    pfree(p, 1);

    return new_p;
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

fn pfree_array(array p)
{
	for(int i = 0; p[i] != NULL; i++)
		pfree(p[i], 1);
}

fn _pfree(any ptr) { pfree(ptr, 1); }
fn pfree(any ptr, int clean)
{
    if (!ptr) return;

    __meta__ *m = __get_meta__(ptr);

    int payload = m->size ? m->size * m->length : m->length;
    int total   = payload + HEAP_META_SZ;

	if(clean)
    	mem_set(m, 1, total);

    used_mem -= total;

	if(HEAP_DEBUG)
	{
    	char buff[100];
    	ptr_to_str(m, buff);
    	print("[ + ] Freeing memory block @ "), println(buff);
    }
}

fn uninit_mem()
{
	if(HEAP_DEBUG)
		println("[ + ] Uninitializing");
	__syscall__((long)_HEAP_, _HEAP_PAGE_, 0, 0, 0, 0, _SYS_MUNMAP);
}
