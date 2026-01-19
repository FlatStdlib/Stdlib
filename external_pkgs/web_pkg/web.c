#include "headers/libweb.h"

handler_t index_page()
{
	
}

int entry()
{
	toggle_debug_mode();
	cws_t ws = init_web_server(NULL, 50);
	if(!ws)
	{
		println("error, unable to put up the webserver!");
		return 1;
	}
	println("Webserver up @ localhost:"), _printi(50), print("\n");

	web_append_route(ws, create_route("index", "/", (handler_t)index_page, 0));
	for(int i = 0; i < ws->route_count; i++)
		print("Path: "), println(ws->routes[i]->path);

	char n[1024];
	int bytes = get_input(n, 1024);
	return 0;
}
