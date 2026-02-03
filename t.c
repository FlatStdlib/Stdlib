#include <fsl.h>

public fn create_output(string search, string comment, string alias)
{
	if(str_len(search) > 0) {
		if(find_string(alias, search) == -1)
			return;

		println("\x1b[32m======================================================\x1b[39m");
	}


	_printf("Alias: %s\n%s\n", alias, comment);
	println("\x1b[32m======================================================\x1b[39m");
}

char SEARCH[1024] = {0};
public bool entry(i32 argc, string argv[])
{
	if(argc >= 2)
		mem_cpy(SEARCH, argv[1], str_len(argv[1]));
	uninit_mem();
	set_heap_sz(_HEAP_PAGE_ * 8);
	init_mem();
	fd_t file = open_file("fsl.h", 0, 0);
	if(!file)
		fsl_panic("err");

	i32 sz = file_content_size(file);
	string buff = allocate(0, sz + 1);
	if(file_read(file, buff, sz) <= 0)
		fsl_panic("unable to read header...!");

	file_close(file);
	int line_count = 0;
	sArr lines = split_lines(buff, &line_count);

	_printf("File Size: %d | Lines: %d\n", (void *)&sz, (void *)&line_count);
	char COMMENT[1024];
	bool capture = false;
	for(int i = 0;  i < line_count; i++)
	{
		if(!lines[i])
			break;

		if(capture == false && i + 1 < line_count && find_string(lines[i], "/*") > -1 && find_string(lines[i + 1], "[@DOC]") > -1)
		{
			capture = true;
			i++;
			continue;
		}

		if(capture && find_string(lines[i], "*/") > -1)
		{
			create_output(SEARCH, COMMENT, lines[i + 1]);
			memzero(COMMENT, 1024);
			capture = false;
		}

		if(capture) {
			for(int c = 0; c < 2; c++)
				trim_char_idx(lines[i], 0);
			str_append_array(COMMENT, (ptr []){lines[i], "\n", NULL});
		}

		pfree(lines[i], 1);
	}
	return 0;
}
