#include <fsl.h>

string HTTP_DATA = "HTTP/1.1 200 OK\r\n"
					"Content-type: html\r\n"
					"Content-length: 13\r\n\r\nHello World\r\n";

typedef struct {
	string 	req_type;
	string 	route;
	string 	req_version;
	map_t 	headers;	// Header(s) map
	map_t 	cookies;	// Cookie(s) map
	map_t 	GET;		// GET Params
	map_t 	POST;		// POST Params/Data

	string 	body;		// Request Body (Holding POST data)
	sArr 	lines;
} cwr_t;

typedef struct {
	string name;
	string route;
	// req_t type; 		// Acceptable request types
	string content; 	// Loaded Content - Editable
} route_t;

typedef route_t **router_t;

typedef struct {
	sock_t 		con;
	router_t 	router;
} cws_t;

cwr_t parse_request(string req)
{
	cwr_t r, empty;
	memzero(&r, sizeof(cwr_t));

	if(!req)
		return empty;

	i32 argc = 0;
	r.lines = split_string(req, '\n', &argc);
	if(argc <= 1 || !r.lines)
		return empty;

	_printf("Lines: %d\n", (ptr)&argc);
	i32 arg_c = 0;
	sArr args = split_string(r.lines[0], ' ', &arg_c);
	if(arg_c < 3 || !args)
		return empty;

	print_args((string []){"Request: ", args[0], " ", args[1], " ", args[2], "\n", NULL});
	pfree_array((array)args);

	r.body = allocate(0, 4096);
	r.headers = init_map();
	int stop_headers = 0;
	int i = 0;
	for(i = 1; i < argc; i++)
	{
		string line = r.lines[i];
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
			map_append(r.headers, key, value);
			continue;
		}

		str_append(r.body, line);
	}

	_printf("Body: \n%s\n", r.body);
	if(r.headers->len > 0)
		return r;

	return empty;
}

void req_destruct(cwr_t *r)
{
	if(r->body)
		_pfree(r->body);

	if(r->headers)
		map_destruct(r->headers);

	if(r->lines)
		pfree_array((array)r->lines);
}

public int entry()
{
	uninit_mem();
	set_heap_sz(_HEAP_PAGE_ * 40);
	init_mem();
	_printf("Heap Size: %d\n", (ptr)&_HEAP_PAGE_);

	sock_t n = listen_tcp(NULL, 80, 999);
	if(n->fd <= 0)
		fsl_panic("Unable to create socket...");

	println("Started");
	while(1)
	{
		sock_t client = sock_accept(n, 1024);

		string buff = sock_read(client);
		if(!buff)
			continue;

		int bytes = __get_size__(buff);
		_printf("Size: %d\n", (ptr)&bytes);

		cwr_t r = parse_request(buff);
		req_destruct(&r);

		sock_write(client, HTTP_DATA);

		pfree(buff, 1);
		sock_close(client);
	}
	return 0;
}
