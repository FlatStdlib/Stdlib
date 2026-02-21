#include <fsl.h>

heap_t my_heap = NULL;
i32 heap_sz = 0;
int entry()
{
	string n = allocate(0, _HEAP_PAGE_ - 150);
	mem_cpy(n, "dick", 4);

	my_heap = _HEAP_;
	heap_sz = _HEAP_PAGE_;
	_HEAP_ = NULL;

	set_heap_sz(_HEAP_PAGE_ * 5);
	init_mem();

	mem_cpy(_HEAP_, my_heap, heap_sz);
	return 0;
}
