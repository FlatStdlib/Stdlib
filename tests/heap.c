#include <stdio.h>
#include <clibp.h>
#define STR_LEN 7
#define _PRESTRING_ "Testing"

fn test_fake_realloc()
{
	string n = allocate(0, 1);

	for(int i = 0; i < STR_LEN; i++)
	{
		n[i] = _PRESTRING_[i];
		string realloc = to_heap(n, i + 1);
		if(!realloc)
		{
			clibp_panic("error, unable to realloc");
		}

		pfree(n, 1);
		n = realloc;
	}

	print_args((string []){"Realloc: ", n, "\n", 0});
	//pfree(n);
}

int entry() {
    /* int array */
    iArr n = allocate(0, 11);
    for(int i = 0; i < 10; i++)
        n[i] = i;

    for(int i = 0; i < 10; i++) _printi(n[i]), print("\n");

    /* str array */
    string name = allocate(0, 10);
    for(int i = 0; i < STR_LEN; i++)
        name[i] = _PRESTRING_[i];

    println((char *)name);
    pfree(n, 1);
    pfree(name, 1);

    test_fake_realloc();
    return 0;
}

int main() {
	printf("Hi from GCC with clib+\n");
}
