#include "headers/libweb.h"

handler_t request_handler(cwr_t wr)
{
	wr->content = sock_read(wr->socket);
	int len = __get_meta__(wr->content)->length;

	println(wr->content);
	if(len < 3)
	{
		println("Malform request...!\n");
		return NULL;
	}

	wr->lines = split_lines(wr->content, &wr->line_count);
	if(wr->line_count == 0)
	{
		println("Malform request\n");
		return NULL;
	}

	if(find_string(wr->lines[0], "GET")) {
		wr->type = _rGET;
	} else if(find_string(wr->lines[0], "POST"))
	{
		wr->type = _rPOST;
	} else if(find_string(wr->lines[0], "HEAD"))
	{
		wr->type = _rHEAD;
	}

	if(find_string(wr->lines[0], "/fav.ico") > -1)
	{
		sock_close(wr->socket);
		return NULL;
	}
    /*
        TODO;
            work on the route handler
            Parse wr->lines[0] for req info
            find path handler or reject request with status code 
    */
	char *fake = "HTTP/1.1 200 OK\r\n"
				 "Content-Type: text/html;charset=UTF-8\r\n"
				 "Content-length: 15\r\n"
				 "Connection: close\r\n\r\n"
				 "Hello World\r\n\r\n";

	print("Line Info: "), println(wr->lines[0]);
	int r = find_route(_WEB_, "/");
	if(r == -1) {
		println("Error");
		return NULL;
	}
	print("Route Found: "), _printi(r), print("\n");
	sock_write(wr->socket, fake);
	return NULL;
}

fn parse_request(cwr_t wr)
{
	bool capture_body = false;

	int len = __get_meta__(wr->content)->length;
	wr->body = allocate(0, len);

	/* Parse Headers and Get body, Skip first line */
	wr->headers = init_map();
	for(int i = 1; i < wr->line_count; i++)
	{
		if(!wr->lines[i])
			break;

		/* After headers which is an empty line, Capture Body */
		if(is_empty(wr->lines[i])) {
			capture_body = true;
			continue;
		} else if(is_empty(wr->lines[i]) && capture_body) { // Break after body
			break;
		}
		
		/* Capture body after header */
		if(capture_body)
		{
			str_append(wr->body, wr->lines[i]);
			continue;
		}

		/* Parse Header Line */
		if(find_string(wr->lines[i], ":") > -1)
		{

			int arg_c = 0;
			sArr args = split_string(wr->lines[i], ':', &arg_c);

			if(arg_c < 2)
			{
				continue;
			}

			int pos = find_char(wr->lines[i], ':');
			map_append(wr->headers, args[0], wr->lines[i] + pos);
			pfree_array((array)args);
		}
	}
}

fn parse_post(cwr_t wr)
{
	if(!wr)
		return;

	if(find_char(wr->body, '=') == -1)
		return;

	wr->post = init_map();
	if(find_char(wr->body, '&') > -1)
	{
		int arg_c = 0;
		sArr args = split_string(wr->body, '&', &arg_c);
		if(!arg_c)
			return;

		for(int i = 0; i < arg_c; i++)
		{
			int argc = 0;
			sArr arg = split_string(args[i], '=', &argc);
			if(!arg || !argc)
				continue;

			map_append(wr->post, arg[0], arg[1]);
		}

		pfree_array((void *)args);
	}

	int argc = 0;
	sArr args = split_string(wr->body, '=', &argc);
	if(!args || !argc)
		return;

	map_append(wr->post, args[0], args[1]);
	pfree_array((void *)args);
}

fn parse_get_parameters(cwr_t wr)
{
	if(!wr)
		return;

	int pos = find_char(wr->uri, '?');
	if(pos == -1)
		return;

	wr->get = init_map();
	string parameters = wr->body + pos;
	if(find_char(parameters, '&')) {
		int argc = 0;
		sArr params = split_string(parameters, '&', &argc);
		if(!argc || !params)
			return;

		for(int i = 0; i < argc; i++)
		{
			int arg_c = 0;
			sArr field = split_string(params[i], '=', &arg_c);
			if(!arg_c || !field)
				continue;

			map_append(wr->get, field[0], field[1]);
			pfree_array((void *)field);
		}
	}

}
