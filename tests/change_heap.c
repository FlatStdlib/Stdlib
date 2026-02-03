#include <libbase.h>

heap_t my_heap = NULL;

public int entry(int argc, char *argv[])
{
	toggle_debug_mode();
	my_heap = _HEAP_;
	_HEAP_ = NULL;
	init_mem();
	_printf("Size: %d | %p -> %p\n", (void *)&_HEAP_PAGE_, my_heap, _HEAP_);
	_HEAP_ = str_dup("dick");
	my_heap = str_dup("fag");

	_printf("%s -> %s\n", _HEAP_, my_heap);
	return 0;
}
