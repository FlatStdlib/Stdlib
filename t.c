//#define DISABLE_AUTO_ARCH_DET	// Disable Arch Auto Detection
#define _STANDARD_MEM_SZ_
#include <clibp.h>				// Include clibp
#include <allocator.h>

/* Keep Types Stored */
#define SERVER_APP				// Keep All TYPES stored in metadata

fn_t entry()
{
	char buff[1024];
	HEAP_DEBUG = 1;
	init_mem();

    _sock_t s = listen_tcp(NULL, 8080, 999);
    if(s.fd > 0) {
		print("Socket Created....!\n");
    }

    __exit(1);
    if(!__is_heap_init__())
    {
        print("ERROR\n");
    }

	fd_t fd = open_file("q.c", O_RDONLY);
	if(fd <= 0)
	{
		print("[ x ] Error, Unable to open file...!\n");
		__exit(1);
	}

	print("[ + ] Open File: "), printi(fd), print("\n");
	int size = file_content_size(fd);
    print("Content Length: "), _printi(size), print("\n");

	print("Allocating....\n");
	str test = allocate(1, size + 1);
    if(test == NULL) {
        print("[ - ] Error, Unable to allocate memory...!\n");
		__exit(1);
    }

	__meta__ *info = __get_meta__(test);
	_printi(info->length), print("\n");
	_printi(info->id), print("\n");
    if(info->id == 0x7C)
    {
		print("Heap Chunk\n");
		__exit(0);
    }

	file_read(fd, test, size);

	int c = str_len(test);
	print("Len: "), printi(c), print("\n");
	print("Content: "), print(test);


	// if(__is_ptr_heap__(test))
	// {
	// 	print("Heap Detected\n");
	// }

	uninit_mem();
	__exit(0);
}
