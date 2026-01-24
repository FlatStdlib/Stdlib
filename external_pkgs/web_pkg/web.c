#include "headers/libweb.h"

handler_t test_page(route_t r, cwr_t wr) {
	// println(r->template);
	send_response(wr, (_response){ OK, 0, 0, r->template}, 1);
}

handler_t index_page(route_t r, cwr_t wr) {
	send_response(wr, (_response){ OK, 0, 0, "Hello World!" }, 0);
}

int entry() {
	toggle_debug_mode();
	uninit_mem();
	set_heap_sz(_HEAP_PAGE_ * 5);
	init_mem();
	cws_t ws = init_web_server(NULL, 80);
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

	web_append_route(ws, create_route(
		"test",
		"/test",
		(handler_t)test_page,
		1
	));

	if(!get_html_template(ws->routes[1], "index.html")) 
		clibp_panic("unable to read resource ")

	start_web_server(ws, 0);
	return 0;
}

int main() { 
	set_heap_sz(4096 * 5);
	init_mem();
	return entry();
	uninit_mem();
}