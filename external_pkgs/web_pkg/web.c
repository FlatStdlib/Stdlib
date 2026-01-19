#include "headers/libweb.h"

handler_t index_page()
{
	
}

int entry()
{
	toggle_debug_mode();
	cws_t ws = init_web_server(NULL, 40);
	if(!ws)
	{
		println("error, unable to put up the webserver!");
		return 1;
	}
	println("Webserver up @ localhost:"), _printi(40), print("\n");

	web_append_route(ws, create_route("index", "/", (handler_t)index_page, 0));
	char n[1024];
	int bytes = get_input(n, 1024);
	return 0;
}
