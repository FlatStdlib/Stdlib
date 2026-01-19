#pragma once

#include <clibp.h>
#include "enums.h"
#include "router.h"

#ifndef _CLIBP_WEB_H
#define _CLIBP_WEB_H

typedef struct
{
	sock_t			socket;
	request_t		type;
	status_code		status;
	string			path;
	string			uri;
	string			http_version;

	map_t			headers;
	map_t			post;
	map_t			get;

	string 			content;
	string			body;
	sArr			lines;
	i32				line_count;
	thread_t		thread;
} _cwr;

typedef _cwr 		cwr;
typedef _cwr 		*cwr_t;

typedef struct
{
	/* Web server's socket info */
	string 			ip;
	i32 			port;
	sock_t			connection;
	thread_t		thread;

	/* Routes */
	handler_t 		middleware;
	rArr			routes;
	i32				route_count;
} _cws;


typedef _cws 		cws;
typedef _cws 		*cws_t;

extern cws_t _WEB_;

cws_t init_web_server(string ip, i32 port);
handler_t listen_for_request(cws_t ws);
fn web_append_route(cws_t ws, route_t route);
int find_route(cws_t ws, string route);

// parser.c
handler_t request_handler(cwr_t wr);
fn parse_request(cwr_t wr);
fn parse_post(cwr_t wr);
fn parse_get_parameters(cwr_t wr);

// route.c
route_t create_route(string name, string path, handler_t fnc, int view_only);
#endif
