#include <fsl.h>

string HTTP_DATA = "HTTP/1.1 200 OK\r\n"
					"Content-type: html\r\n"
					"Content-length: 13\r\n\r\nHello World\r\n";


map_t headers;

void parse_request(string req)
{
	if(!req)
		return;

	i32 argc = 0;
	sArr lines = split_string(req, '\n', &argc);
	if(argc <= 1 || !lines)
		return;

	_printf("Lines: %d\n", (ptr)&argc);
	i32 arg_c = 0;
	sArr args = split_string(lines[0], ' ', &arg_c);
	if(arg_c < 3 || !args)
		return;

	print_args((string []){"Request: ", args[0], " ", args[1], " ", args[2], "\n", NULL});

	string _body = allocate(0, 4096);
	headers = init_map();
	int stop_headers = 0;
	int i = 0;
	for(i = 1; i < argc; i++)
	{
		string line = lines[i];
		if(!line) break;

		if(line[1] == '\0') {
			stop_headers = 1;
			continue;
		}

		if(!stop_headers)
		{
			i32 pos = find_char(line, ':');
			if(pos == -1)
				continue;

			string key = line;
			line[pos] ='\0';

			string value = line + pos + 2;
			_printf("Key: %s | Value: %s\n", key, value);
			continue;
		}

		str_append(_body, line);
		_pfree(line);
	}

	pfree(lines, 0);
	pfree_array((array)args);
	_printf("Body: \n%s\n", _body);
}

public int entry()
{
	uninit_mem();
	set_heap_sz(_HEAP_PAGE_ * 5);
	init_mem();

	sock_t n = listen_tcp(NULL, 80, 999);
	if(n->fd <= 0)
		fsl_panic("Unable to create socket...");

	println("Started");
	while(1)
	{
		sock_t client = sock_accept(n, 1024);

		string buff = sock_read(client);
		int bytes = __get_size__(buff);
		_printf("Size: %d\n", (ptr)&bytes);

		parse_request(buff);

		sock_write(client, HTTP_DATA);
		pfree(buff, 1);
		sock_close(client);
	}
	return 0;
}
