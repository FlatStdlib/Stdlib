#include "../../headers/fsl.h"

#if defined(_WIN32) || defined(_WIN64)
    #define __FSL_WINDOWS__

#include <stdio.h>
#include <string.h>

int __FSL_DEBUG__ = 0;
char _OUTPUT_[1024] = {0};
char _EXTERNAL_[1024] = {0};

#define fsl_panic(msg) 	\
	__fsl_panic(msg, __FILE__, __LINE__);

#define fsl_warning(msg) \
	__fsl_warning(msg, __FILE__, __LINE__);

public fn __fsl_warning(string msg, string file, int line)
{
    
	if(__FSL_DEBUG__)
		printf("%s:%d -> ", file, line);
        
    printf("%s:%d -> \x1b[34mwarning\x1b[39m: %s\n", file, line, msg);
}

public fn __fsl_panic(string msg, string file, int line)
{
	if(__FSL_DEBUG__)
		printf("%s:%d -> ", file, line);

	printf("\x1b[31merror\x1b[39m: %s\n", msg);
	_exit(1);
}

void printi(int num) { printf("%d", num); }
void _printi(int num) { printf("%d", num); }
void print(string x) { printf("%s", x); }
void println(string x) { printf("%s\n", x); }

public fn ptr_to_str(ptr p, string out)
{ sprintf(out, "%p", p); }

#endif