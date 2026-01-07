#include <clibp.h>

sArr ARGV;
int ARGC;

/* Fetch all c files provided in cmdline */
sArr get_all_c_files()
{
	int count = 0;
	for(int i = 0; i < ARGC; i++)
	{
		int len = str_len(ARGV[i]);
		if(!len) continue;

		if(ARGV[i][len - 1] == 'c' && ARGV[i][len - 2] == '.')
			count++;
	}

	if(!count) return nullptr_t;
	sArr files = allocate(sizeof(string), count + 1);
	if(!files) {
		clibp_panic("error, unable to allocate memory");
		return nullptr_t;
	}

	int idx = 0;
	for(int i = 0; i < ARGC; i++)
	{
		int len = str_len(ARGV[i]);
		if(!len) continue;

		if(ARGV[i][len - 1] == 'c' && ARGV[i][len - 2] == '.')
			files[idx++] = ARGV[i];
	}

	files[idx] = NULL;
	return files;
}

/* Fetch all object files provided in cmdline */
sArr get_all_object_files()
{
	int count = 0;
	for(int i = 0; i < ARGC; i++)
	{
		if(!ARGV[i]) break;

		int len = str_len(ARGV[i]);
		if(!len) continue;

		if(ARGV[i][len - 1] == 'o')
			count++;
	}

	if(count == 0) {
		println("[ x ] error, no object files found");
		return NULL;
	}

	sArr n = allocate(sizeof(string), count + 1);
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
