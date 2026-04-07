#include <fsl.h>
#undef int

typedef struct
{
	string 	filename;
	string 	content;
	sArr  	lines;
	fd_t 	handle;
} _hdr_;

int get_symbol_comments(_hdr_ hdr, int idx)
{
	for(int i = idx; i != 0; i--)
	{
		if(!hdr.lines[i]) break;
		
		str_strip(hdr.lines[i]);
		if(hdr.lines[i][0] == '/' && hdr.lines[i][1] == '*')
			return i;
	}

	return -1;
}

_hdr_ open_hdr_file(string name, string query)
{
	_hdr_ hdr = {0};
	// mem_set(&hdr, 0, sizeof(_hdr_));
	hdr.handle = open_file(name, 0, 0);
	if(!hdr.handle)	
		fsl_panic("unable to open header file...!");

	size_t size = file_content_size(hdr.handle);
	if(size == 0)
		fsl_panic("empty header file....!");
	
	_printf("File Size: %d\n", (void *)&size);
	hdr.content = allocate(0, size + 1);
	if(__get_size__(hdr.content) != size + 1)
		fsl_panic("Unable to allocate data...!");

	int bytes = file_read(hdr.handle, hdr.content, size);

	int line_c = 0;
	hdr.lines = split_lines(hdr.content, &line_c);

	_printf("File Lines %d\n", (void *)&line_c);

	for(int i = 0; i < line_c; i++) {
		if(find_string(hdr.lines[i], query) > -1) {
			int start = get_symbol_comments(hdr, i) + 2;
			if(start > -1)
			{
				print("\x1b[32mInfo:\x1b[39m\n");
				for(int c = start; c < i && hdr.lines[c][0] != '*'; c++)
					print("\t"), println(hdr.lines[c]);
			}

			print("\x1b[32mSymbol:\x1b[39m\n\t");
			/* print("["), printi(i), print("]: "), */ println(hdr.lines[i]);
		}
	}

	pfree_array((array)hdr.lines);
	return hdr;
}

char search_query[1024];

// fsl --output t t.c --cflags -nostdinc -I headers
public int entry(int argc, string argv[])
{
	if(argc < 2)
		fsl_panic("Invalid arguments provided...!\nUsage: ./search <query>");

	memzero(&search_query, 1024);
	if(str_len(argv[1]) < 1)
		fsl_panic("Invalid argument");

	mem_cpy(search_query, argv[1], str_len(argv[1]));
	// toggle_debug_mode();
	uninit_mem();
	set_heap_sz(_HEAP_PAGE_ * 8);
	init_mem();

	if(!__is_heap_init__())
		fsl_panic("Heap not initialized!");

	_hdr_ hdr = open_hdr_file("/usr/local/include/fsl.h", search_query);
	return 0;
}