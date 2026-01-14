#include <clibp.h>
#include <stdbool.h>

typedef enum {
    CONTINUEE                       = 100,
    SWITCH_PROTOCOL                 = 101,
    PROCESSING                      = 102,
    EARLY_HINT                      = 103,

    OK                              = 200,
    CREATED                         = 201,
    ACCEPTED                        = 202,
    NON_AUTHORIZED_INFO             = 203,
    NO_CONTENT                      = 204,
    RESET_CONTENT                   = 205,
    PARTIAL_CONTENT                 = 206,
    MULTI_STATUS                    = 207,
    ALREADY_REPRORTED               = 208,
    IM_USED                         = 226,

    MULTIPLE_CHOICES                = 300,
    MOVED_PERMANENTLY               = 301,
    FOUND                           = 302,
    SEE_OTHER                       = 303,
    NOT_MODIFIED                    = 304,
    USE_PROXY                       = 305,
    SWITCH_PROXY                    = 306,
    TEMP_REDIRECT                   = 307,
    PERM_REDIRECT                   = 308,

    BAD_REQUEST                     = 400,
    UNAUTHORIZED                    = 401,
    PAYMENT_REQUIRED                = 402,
    FORBIDDEN                       = 403,
    NOT_FOUND                       = 404,
    METHOD_NOT_ALLOWED              = 405,
    NOT_ACCEPTABLE                  = 406,
    PROXY_AUTH_REQUIRED             = 407,
    REQUIRE_TIMEOUT                 = 408,
    CONFLICT                        = 409,
    GONE                            = 410,
    LENGTH_REQUIRED                 = 411,
    PRECONDITION_FAILED             = 412,
    PAYLOAD_TOO_LARGE               = 413,
    URI_TOO_LONG                    = 414,
    UNSUPPORTED_MEDIA_TYPE          = 415,
    RANGE_NOT_SATISFIABLE           = 416,
    EXPECTATION_FAILED              = 417,
    IM_A_TEAPOT                     = 418,
    MISDIRECTED_REQUEST             = 421,
    UNPROCESSABLE_ENTITY            = 422,
    LOCKED                          = 423,
    FAILED_DEPENDENCY               = 424,
    TOO_EARLY                       = 425,
    UPGRADE_REQUIRED                = 426,
    PROCONDITION_REQUIRED           = 428,
    TOO_MANY_REQUEST                = 429,
    REQ_HEADER_FIELD_TOO_LARGE      = 431,
    UNAVAILABLE_FOR_LEGAL_REASON    = 451,

    INTERNAL_SERVER_ERR             = 500,
    NOT_IMPLEMENTED                 = 501,
    BAD_GATEWAY                     = 502,
    SERVER_UNAVAILABLE              = 503,
    GATEWAY_TIMEOUT                 = 504,
    HTTP_VERSION_NOT_SUPPORTED      = 505,
    VARIANT_ALSO_NEGOTIATES         = 506,
    INSUFFICIENT_STORAGE            = 507,
    LOOP_DETECTED                   = 508,
    NOT_EXTENDED                    = 510,
    NETWORK_AUTHENTICATION_REQUIRED = 511
} http_status_code;

typedef enum
{
	_rNULL 				= 0,
	_rGET 				= 1,
	_rPOST 				= 2,
	_rHEAD 				= 3
} request_t;

typedef struct
{
	string				name;
	string				path;
	handler_t			handle;

	bool				parse_status_code;
	bool				parse_headers;
	bool				code;
} _route;

typedef _route 			route;
typedef _route 			*route_t;
typedef _route 			**rArr;

typedef struct
{
	sock_t				socket;
	request_t			type;
	http_status_code	status;
	string				path;
	string				http_version;

//	map_t				headers;
//	map_t				post;

	string 				content;
	sArr				lines;
	i32					body_line;
	thread_t			thread;
} _cwr;

typedef _cwr 			cwr;
typedef _cwr 			*cwr_t;

typedef struct
{
	/* Web server's socket info */
	string 				ip;
	i32 				port;
	sock_t				connection;
	addr_in 			addr;
	thread_t			thread;

	/* Routes */
	handler_t 			middleware;
	rArr				routes;
	i32					route_count;
} _cws;


typedef _cws 		cws;
typedef _cws 		*cws_t;

handler_t listen_for_request(cws_t ws);
handler_t request_handler(cwr_t wr);

cws_t init_web_server(string ip, i32 port)
{
	cws webs = {
		ip,
		port,
		listen_tcp(NULL, 8080, 999),
		{0},
		NULL,
		NULL,
		0
	};

	cws_t ws = to_heap(&webs, sizeof(_cws));
	ws->thread = allocate(0, sizeof(_thread));
	if(!ws->thread)
		clibp_panic("error, unable to allocate mem");

	*ws->thread = start_thread((handler_t)listen_for_request, ws, 0);
	return ws;
}

handler_t listen_for_request(cws_t ws) {
	sock_t client;
	while(1)
	{
		if(!(client = sock_accept(ws->connection, 1024)))
			continue;

		cwr_t wr = allocate(0, sizeof(_cwr));
		if(!wr)
			clibp_panic("error, unable to allocate new request struct");
		wr->socket = client;
		wr->thread = allocate(0, sizeof(_thread));
		*wr->thread = start_thread((handler_t)request_handler, wr, 0);
	}
}

handler_t request_handler(cwr_t wr)
{
	string data = sock_read(wr->socket);
	int len = str_len(data);

	if(len < 3)
	{
		println("Malform request...!\n");
		return NULL;
	}

	if(find_string(data, "GET")) {
		wr->type = _rGET;
	} else if(find_string(data, "POST"))
	{
		wr->type = _rPOST;
	}

}

int entry()
{
	cws_t ws = init_web_server(NULL, 8080);
	if(!ws)
	{
		println("error, unable to put up the webserver!");
		return 1;
	}
	println("Webserver up @ localhost:"), _printi(8080), print("\n");
	return 0;
}
