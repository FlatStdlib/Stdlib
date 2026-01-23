#include "headers/libweb.h"

field_t DEFAULT_HEADERS[] = {
	&(_field){ .key = "Content-Type", .value = "text/html;charset=UTF-8" },
	&(_field){ .key = "Connection", .value = "close" },
	NULL
};

typedef struct {
	status_code code;
	map_t 		headers;
	map_t 		cookie;
	string 		content;
} _response;

fn send_response(cwr_t wr, _response r)
{
	// TODO; change this hardcoded size shit
	string ctx = allocate(0, 4096);

	str_append(ctx, "HTTP/1.1 ");
	str_append_int(ctx, r.code);
	str_append(ctx, " ");
	str_append(ctx, status_code_to_string(r.code));
	str_append(ctx, "\r\n");

	int body_len = r.content ? str_len(r.content) + 3: 0;
	if(r.headers)
	{
		if(body_len > 0)
		{
			string n = int_to_str(body_len);
			str_append(ctx, "Content-Length: ");
			str_append(ctx, n);
			pfree(n, 1);
		}

		for(int i = 0; i < r.headers->len; i++)
		{
			str_append(ctx, r.headers->fields[i]->key);
			str_append(ctx, ":");
			str_append(ctx, r.headers->fields[i]->value);
			str_append(ctx, "\r\n");
		}
	} else {
		if(body_len > 0)
		{
			string n = int_to_str(body_len);
			str_append(ctx, "Content-Length: ");
			str_append(ctx, n);
			str_append(ctx, "\r\n");
			pfree(n, 1);
		}

		for(int i = 0; DEFAULT_HEADERS[i] != NULL; i++)
		{
			str_append(ctx, DEFAULT_HEADERS[i]->key);
			str_append(ctx, ": ");
			str_append(ctx, DEFAULT_HEADERS[i]->value);
			str_append(ctx, "\r\n");
		}
	}

	str_append(ctx, "\r\n");
	if(r.cookie)
	{
		// implement this shit
	}


	if(r.content) {
		str_append(ctx, r.content);
		str_append(ctx, "\r\n\r\n");
	}

	sock_write(wr->socket, ctx);
	if(__CLIBP_DEBUG__)
		print("Generated Response: "), println(ctx);

	pfree(ctx, 1);
}

handler_t index_page(cwr_t wr) {
	send_response(wr, (_response){ OK, 0, 0, "Hello World!" });
}

int entry() {
	cws_t ws = init_web_server(NULL, 50);
	if(!ws)
	{
		println("error, unable to put up the webserver!");
		return 1;
	}

	_printf("Web server running @ http://localhost:%d\n", (void *)&ws->port);

	web_append_route(ws, create_route(
		"index",
		"/",
		(handler_t)index_page,
		1
	));

	start_web_server(ws, 0);
	return 0;
}

int main() { 
	set_heap_sz(4096 * 5);
	init_mem();
	return entry();
	uninit_mem();
}