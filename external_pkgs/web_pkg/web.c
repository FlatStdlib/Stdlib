#include "headers/libweb.h"

handler_t test_page(route_t r, cwr_t wr) {
	send_response(wr, (_response){ OK, 0, 0, r->template}, 1);
}

handler_t index_page(route_t r, cwr_t wr) {
	send_response(wr, (_response){ OK, 0, 0, 
		(
			"<html>\n" 
			"<head>\n"
			"<title>Algo1337</title>\n"
			"</head>\n"
			"<body>\n"
			"<p>Hi, I'm Algo! This was made using libweb<br />A production ready web server for the libzero</p>\n"
			"<p>libzero: <a href=\"https://github.com/libzero\">Github</a></p>\n"
			"<p>libweb: <a href=\"https://github.com/libzero/libweb\">Github Repo</a></p>\n"
			"</body>\n"
			"</html>\n"
		)
	}, 0);
	println("SENT");
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

// int main() { 
// 	set_heap_sz(4096 * 5);
// 	init_mem();
// 	return entry();
// 	uninit_mem();
// }