#include <fsl.h>

int entry()
{
	string gay = allocate(0, 10);
	printi(sizeof(__meta__)), println(NULL);
	printi(((__meta__ *)((char *)gay - HEAP_META_SZ))->id), println(NULL);
	printi(0x7C), println(NULL);
	printi(sizeof(size_t)), println(NULL);
	printi(sizeof(int)), println(NULL);
	println("HELLO WORLD");
	return 0;
}