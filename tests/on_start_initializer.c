#include <fsl.h>

void _atexit(void *(*handler)());

void *_on_exit()
{
	println("Exiting...");
}

int entry()
{
	_atexit(_on_exit);
	return 0;
}
