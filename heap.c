#include <fsl.h>

heap_t my_heap = NULL;
i32 heap_sz = 0;
int entry()
{
	mem_cpy(_HEAP_, "dick", 4);

	my_heap = _HEAP_;
	heap_sz = _HEAP_PAGE_;
	_HEAP_ = NULL;

	set_heap_sz(_HEAP_PAGE_ * 1);
	init_mem();

	if(!__is_heap_init__())
		fsl_panic("Heap is not initialized...!");

	mem_cpy(_HEAP_, "FAG", 3);
	_printf("Old Heap: %s\n", my_heap);
	_printf("New Heap: %s\n", _HEAP_);
	return 0;
}
