#include "../headers/clibp.h"

size_t _HEAP_PAGE_SZ_ = 1024;
heap_t _HEAP_ = NULL;
static int used_mem = 0;
int HEAP_DEBUG = 0;
int HEAP_META_SZ = sizeof(__meta__);

#define TEST_NUM 4086
//void init_mem() {
//	_HEAP_ = (void *)_syscall(_SYS_MMAP, 0, _HEAP_PAGE_SZ_, 0x01 | 0x02, 0x02 | 0x20, -1, 0);
//	if(_HEAP_ == ((void *)-1))
//	{
//		print("[ - ] Error, Unable to initialize heap....!\n");
//	}
//
//	//mem_set(_HEAP_, 0, _HEAP_PAGE_SZ_);
////	((char *)_HEAP_)[0] = '\0';
//
//	print("Allocated: "), _printi(_HEAP_PAGE_SZ_), print("\n");
//	if(HEAP_DEBUG)
//		print("[ + ] HEAP INITIALIZED!\n");
//}
//
//void uninit_mem() {
//	__syscall(11, _HEAP_PAGE_SZ_, 0, 0, -1, -1, -1);
//}
//
int __get_total_mem_used__(void) { return used_mem; }
int __is_heap_init__() { return (_HEAP_ ? 1 : 0); }
//
//static int find_space(int space)
//{
//    for (int num = 0; num < _HEAP_PAGE_SZ_ - space - 1; num++)
//	{
//        int free = 1;
//        for (int i = 0; i < space; i++)
//		{
//            if (((char *)_HEAP_)[num + i] != '\0')
//			{
//                free = 0;
//                break;
//            }
//        }
//
//        if (free)
//		{
//            return num;
//        }
//    }
//
//    return -1;
//}
//
//any allocate(int sz, int len) {
//	if(!len)
//		return NULL;
//
//	int space_left = _HEAP_PAGE_SZ_ - used_mem;
//	if(space_left < 10) {
//		print("[ x ] Error, You cannot create a heap chunk!\n");
//		return NULL;
//	}
//	print("2\n");
//	_printi(space_left), print(":");
//	int mem = (!sz ? len : sz * len) + HEAP_META_SZ;
//	_printi(mem), print("\n");
//	if(space_left < mem)
//		return NULL;
//
//	print("3\n");
//	int spot = find_space(mem);
//	print(">"), _printi(spot);
//	if(spot == -1)
//	{
//		print("[ - ] Unable to find space for chunk!\n");
//		return NULL;
//	}
//	any ptr = _HEAP_ + spot;
//
//	print("3\n");
//	__meta__ c = {
//		.size = sz,
//		.length = len,
//		.id = 0x7C
//	};
//
////	mem_cpy(ptr, &c, HEAP_META_SZ);
//
//	used_mem += mem;
//	return (any)(((char *)ptr) + HEAP_META_SZ);
//}

void init_mem() {
    // mmap: addr=0, length=_HEAP_PAGE_SZ_, prot=RW, flags=PRIVATE|ANONYMOUS, fd=-1, offset=0
    _HEAP_ = (void *)_syscall(9, 0, _HEAP_PAGE_SZ_, 0x1|0x2, 0x2|0x20, -1, 0);

    if (_HEAP_ == (void *)-1) {
        print("[ - ] Error, mmap failed!\n");
        return;
    }

    // Clear the heap to mark all memory as free
//    mem_set(_HEAP_, 0, _HEAP_PAGE_SZ_);
//	((char *)_HEAP_)[0] = '\0';

    if (HEAP_DEBUG)
        print("[ + ] Heap initialized!\n");
}

static int find_space(int space)
{
    for (int i = 0; i <= _HEAP_PAGE_SZ_ - space; i++) {
        int free = 1;
        for (int j = 0; j < space; j++) {
            if (((char *)_HEAP_)[i + j] != 0) {
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

    int space_left = _HEAP_PAGE_SZ_ - used_mem;
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

    // Copy meta into allocated block
    mem_cpy(ptr, &c, HEAP_META_SZ);

    // Mark memory as used (fill with 1s or some non-zero)
    mem_set(ptr + HEAP_META_SZ, 1, mem_needed - HEAP_META_SZ);

    used_mem += mem_needed;

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

void pfree(any ptr) {
	any original = (char *)ptr - HEAP_META_SZ;
	int sz = ((char *)ptr - HEAP_META_SZ)[0];
	int len = ((char *)ptr - HEAP_META_SZ)[1];
	mem_set(original, 0, (!sz ? len : sz * len) + 5);
	used_mem -= (!sz ? len : sz * len) + 5;
}
