/*
	Cmdline parser for clib+ compiler-linker test
*/
#include <clibp.h>

sArr ARGV;
int ARGC;

/* Compiler types supported with clib+ */
#define COMPILER_TYPE_COUNT 3
static string COMPILER_TYPES[] = {
    "/usr/bin/gcc/ -nostdlib -ffreestanding -c",
    "/usr/bin/tcc -nostdlib -ffreestanding -std=c99 -c",
    0
};

/* returns a stack const */
static string get_compiler_type_cmd(string cmd_buffer, string compiler)
{
    for(int i = 0; i < COMPILER_TYPE_COUNT; i++)
        if(find_str(COMPILER_TYPES[i], compiler) > -1)
            return COMPILER_TYPES[i];

    return 0;
}

/* Fetch all c files provided in cmdline */
static sArr get_all_c_files()
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
static sArr get_all_object_files()
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

bool __arr_contains(sArr arr, i32 size, string value)
{
	for(i32 i = 0; i < size; i++)
	{
		if(str_cmp(arr[i], value))
			return true;
	}

	return false;
}

/* gcc_clibp t.c -o t */
int entry(int argc, char *argv[])
{
	println("Loaded from clib+");
	if(argc < 3)
	{
		println("[ x ] error, invalid arguments provided\nUsage: gcc_clibp <file> <-c/-o> <output_file>\nUse '--h' for a list of arguments");
		return 1;
	}

	ARGC = argc;
	ARGV = argv;
	string COMPILE_CMD = allocate(0, 1024);
	i32 idx = 0, skip = 0;

	skip += 2;
	string compiler = get_compiler_type_cmd(
		COMPILE_CMD,
		__arr_contains(ARGV, ARGC, "--tcc") ? "tcc" : "gcc"
	);

	// user must provide a main script before -c or -o then the rest of the files
	if(__arr_contains(ARGV, ARGC, "-c"))
	{
		// exit here since clibp already compile to object file
		skip++;
	}

	if(__arr_contains(ARGV, ARGC, "--strip"))
	{
		// strip the binary
		// CMD: strip <file>
		skip++;
	}

	if(__arr_contains(ARGV, ARGC, "--upx"))
	{
		// compress with upx
		// CMD: upx -9 <file>
		skip++;
	}

	for(int i = 0; i < ARGC; i++)
	{
		char n[100];
		__sprintf(n, "[ %d ]: %p -> %s\n", (void *)&i, ARGV[i], ARGV[i]);
		print(n);
	}

	return 0;
}

/* gcc t.c -o t -lclibp */
int main(int argc, char *argv[])
{
	println("Loaded from libc");
	return 0;
}
