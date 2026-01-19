#define __CLIBP_DEBUG__
#include "headers/libweb.h"
cws_t _WEB_ = NULL;
cws_t init_web_server(string ip, i32 port)
{
	cws_t ws = allocate(0, sizeof(_cws));
	if(!ws)
		clibp_panic("Segfault");

	ws->ip = ip;
	ws->port = port;
	ws->connection = listen_tcp(NULL, port, 99);
	ws->middleware = NULL;
	ws->routes = allocate(0, sizeof(_route *));
	ws->route_count = 0;

	// listen_for_request(ws);
	ws->thread = allocate(0, sizeof(_thread));
	if(!ws->thread)
		clibp_panic("error, unable to allocate mem");

	*ws->thread = start_thread((handler_t)listen_for_request, ws, 0);
	_WEB_ = ws;
	return ws;
}

handler_t listen_for_request(cws_t ws) {
	sock_t client;
	while(1)
	{
		println("Listening for web requests....!");
		if(!(client = sock_accept(ws->connection, 1024)))
			continue;

		cwr_t wr = allocate(0, sizeof(_cwr));
		if(!wr)
			clibp_panic("error, unable to allocate new request struct");
		wr->socket = client;
		wr->thread = allocate(0, sizeof(_thread));
		*wr->thread = start_thread((handler_t)request_handler, wr, 0);
	}

	println("Exiting...");
}

fn web_append_route(cws_t ws, route_t route)
{
	ws->routes[ws->route_count++] = route;
	ws->routes = reallocate(ws->routes, sizeof(_route *) * (ws->route_count + 1));
}

int find_route(cws_t ws, string route)
{
	if(!ws || !route)
		return -1;

	for(int i = 0; i < ws->route_count; i++)
		if(mem_cmp(ws->routes[i]->path, route, str_len(ws->routes[i]->path)))
			return i;

	return -1;
}