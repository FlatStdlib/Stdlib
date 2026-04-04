#include <fsl.h>

typedef struct
{
	string 	filename;
	string 	content;
	sArr  	lines;
	fd_t 	handle;
} _hdr_;

_hdr_ open_hdr_file(string name)
{
	_hdr_ hdr;
	hdr.handle = open_file(name, 0, 0);
	if(!hdr.handle)	
		fsl_panic("unable to open header file...!");

	size_t size = file_content_size(hdr.handle);
	if(size == 0)
		fsl_panic("empty header file....!");
	
	println("HERE");
	hdr.content = allocate(0, size + 1);
	int bytes = file_read(hdr.handle, hdr.content, size);

	_printf("File Size: %d\nData: %s\n", (void *)&size, hdr.content);
	// int line_c = 0;
	// hdr.lines = split_lines(hdr.content, &line_c);

	// _printf("Lines %d\n", (void *)&line_c);
	// for(int i = 0; i < line_c; i++)
	// 	println(hdr.lines[i]);

	return hdr;
}


public int entry()
{
	toggle_debug_mode();
	// uninit_mem();
	// set_heap_sz(_HEAP_PAGE_ * 2);
	// _printf("%d\n", (void *)&_HEAP_PAGE_); println(NULL);
	// init_mem();

	if(!__is_heap_init__())
		fsl_panic("Heap not initialized!");

	_hdr_ hdr = open_hdr_file("/usr/local/include/fsl.h");
	return 0;
}