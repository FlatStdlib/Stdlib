#include <fsl.h>

typedef struct
{
	string type, alias;
} _symbol;

typedef struct
{
	string 	filename;
	fd_t 	handle;
	sArr 	lines;
	i32 	line_count;
	_symbol	**symbols;
	i32		len;
} _hdr_file;

typedef _hdr_file *hdr;

hdr init_header(string filename)
{
	hdr file = allocate(0, sizeof(_hdr_file));
	if(file)
	{
		file->handle = open_file("/usr/local/include/fsl.h", 0, 0);
	} else {
		file->handle = open_file(filename, 0, 0);
	}

	if(!file->handle)
		fsl_panic("unable to open header file");

	long sz = file_content_size(file->handle);
	string content = allocate(0, sz + 1);
	if(!content)
		fsl_panic("err");

	i32 bytes = file_read(file->handle, content, sz);
	if(bytes <= 0)
		fsl_panic("unable to read header file content");

	file->lines = split_string(content, ' ', &file->line_count);
	if(!file->line_count)
		fsl_panic("unable to split header file lines");

	return file;
}

int entry(i32 argc, string argv[])
{
	if(argc < 2)
	{
		fsl_panic("invalid arguments provided....!\nUsage: fml <alias>");
	}

	hdr file = NULL;
	if(find_string(argv[1], ".h") > -1 || find_string(argv[1], "/") > -1)
	{
		file = init_header(argv[1]);
	} else {
		file = init_header(NULL);
	}

	return 0;
}
