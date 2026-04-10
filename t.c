#include <fsl.h>

static string make_backup_heap()
{
	static char gay[_HEAP_PAGE_];
	return gay;
}

int entry(int argc, string argv[])
{
	
	return 0;
}
