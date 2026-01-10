#include <clibp.h>

void _start();

void _test()
{
	println("HERE");
}

int entry()
{
	__CLIBP_DEBUG__ = 1;
	println("HI");
	return 0;
}

int main()
{
	_start();
	return 0;
}
