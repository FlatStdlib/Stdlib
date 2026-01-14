#include <clibp.h>

int entry()
{
	uninit_mem();
	set_heap_sz(4096 * 20);
	_printi(_HEAP_PAGE_), print("\n");
	init_mem();
	fd_t fd = open_file("a.out", 0, 0);
	if(fd < 0)
	{
		println("[ x ] Error, unable to open file...!");
		return 1;
	}

	int size = file_content_size(fd);
	if(size == 0)
	{
		println("[ x ] Error, no content detect in file...!");
		return 1;
	}

	print("Size: "), _printi(size), print("\n");
	unsigned char *buffer = allocate(0, size + 1);
	int bytes = file_read(fd, buffer, size);
	if(bytes < 0)
	{
		println("[ x ] Error, unable to read file...!");
		return 1;
	}

	print("Bytes Read: "), _printi(bytes), print("\n");
	file_close(fd);
	// 48 c7 45 10 01 00 00
	unsigned char *new_binary = allocate(0, size);
	int idx = 0;

	for(int i = 0; i < size; i++)
	{
		if(buffer[i] == 0x48 && buffer[i + 1] == 0x89 && buffer[i + 2] == 0xC8) {
			new_binary[idx++] = 0xB8;
			new_binary[idx++] = 0x69;
			new_binary[idx++] = 0x00;
			new_binary[idx++] = 0x00;
			new_binary[idx++] = 0x00;
			new_binary[idx++] = 0x0F;
			new_binary[idx++] = 0x05;
			new_binary[idx++] = 0x5D;
			i += 8;
		}

		if(buffer[i] == 0x5D)
		{
			buffer[i] = 0x0F;
			buffer[i + 1] = 0x05;
//			break;
		}

		new_binary[idx++] = buffer[i];
	}

	print("Original size: "), _printi(size), print(" -> "), _printi(idx), print("\n");
	fd_t new_file = open_file("neww", 0, O_WRONLY | O_CREAT | O_TRUNC);
	file_write(new_file, new_binary, idx);
	file_close(new_file);
	return 0;
}
