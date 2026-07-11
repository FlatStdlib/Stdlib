#include <fsl.h>

string HTTP_DATA = "HTTP/1.1 200 OK\r\n"
					"Content-type: html\r\n"
					"Content-length: 15\r\n\r\nHello World\r\n\r\n";

void parse_request(string req)
{
	if(!req)
		return;

	i32 argc = 0;
	sArr lines = split_lines(req, &argc);
	if(argc <= 1 || !lines)
		return;

	i32 arg_c = 0;
	sArr args = split_string(lines[0], ' ', &arg_c);

	if(arg_c < 3 || !args)
		return;

	print_args((string []){args[0], " ", args[1], " ", args[2], " ", NULL});

	map_t headers = init_map();
	string body = allocate(0, 1024);
	int stop_headers = 0;
	for(int i = 1; i < argc; i++)
	{
		string line = lines[i];

		if(line[i] == ' ' && line[i + 1] == ' ')
			stop_headers = 1;

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

		str_append(body, line);
	}

	pfree_array((array)lines);
	_printf("Body: \n%s\n", body);
}

public int entry()
{
	sock_t n = listen_tcp(NULL, 80, 999);
	if(n->fd <= 0)
		fsl_panic("Unable to create socket...");

	println("Started");
	while(1)
	{
		sock_t client = sock_accept(n, 1024);

		string buff = sock_read(client);
		print(buff);
		parse_request(buff);
		sock_write(client, HTTP_DATA);

		sock_close(client);
	}
	return 0;
}
