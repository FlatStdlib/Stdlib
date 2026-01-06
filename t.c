#include <clibp.h>

sArr ARGV;
int ARGC;

void get_all_c_files()
{

}

/* Allocated array with stack pointer */
sArr get_all_object_files()
{
	int count = 0;
	for(int i = 0; i < ARGC; i++)
	{
		int len = str_len(ARGV[i]);
		if(ARGV[i][len - 1] == 'o') {
			count++;
		}
	}

	if(count == 0) {
		println("[ x ] error, no object files found");
		return NULL;
	}

	sArr n = allocate(0, sizeof(str) * (count + 1));
	if(n == NULL) println("error");

	int idx = 0;
	for(int i = 0; i < ARGC; i++) {
		int len = str_len(ARGV[i]);
		if(ARGV[i][len - 1] == 'o') {
			n[idx++] = ARGV[i];
		}
	}

	n[idx] = NULL;
	return n;
}

string create_string(string preset)
{
	int len = str_len(preset);
	string n = allocate(0, len + 1);

	mem_cpy(n, preset, len);
	return n;
}

int entry(int argc, char *argv[])
{
	if(argc == 0)
	{
		println("[ x ] error, unable to get cmdline args");
		return 1;
	}

	ARGC = argc;
	ARGV = argv;

	sArr test = get_all_object_files();
	if(!test)
	{
		println("ERROR");
		return 1;
	}

	println("Object Files");
	for(int i = 0; test[i] != NULL; i++)
		_printi(i), print(": "), println((string)test[i]);

	return 0;
}
