#include <fsl.h>

int entry()
{
	__FSL_DEBUG__ = 1;
	fd_t file = open_file("/etc/os-release", 0, 0);
	if(file < 0)
	{
		fsl_panic("error, unable to open file");
		return 1;
	}

	print("File Descriptor: "), _printi(file), print("\n");

	int size = file_content_size(file);
	print("File Length: "), _printi(size), print("\n");

	string buffer = allocate(0, size + 1);
	if(!buffer)
	{
		fsl_panic("error, unable to allocate memory...!");
		return 1;
	}

	int bytes = file_read(file, buffer, size);
	if(bytes <= 0)
	{
		fsl_panic("error, unable to read file...!");
		return 1;
	}

	println(buffer);
	file_close(file);
	pfree(buffer, 1);
	return 0;
}
