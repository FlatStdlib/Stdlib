#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "fsl.h"

/*
	Used to compile C - Stdlib malloc w/ clib+
*/
#if defined(_C_MALLOC_ALTERNATIVE)
	#define malloc allocate
	#define printf _printf
#endif

void __auto_free__(void *p)
{
 	free(*(void **)p);
 	println("Freeing block....\n");
}

#define AUTO __attribute__((cleanup(__auto_free__)))
