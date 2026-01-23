#include "headers/libweb.h"

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