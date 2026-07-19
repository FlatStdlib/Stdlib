#include <fsl.h>

typedef enum HTTP_VERSIONS {
    HTTP_1_0,
    HTTP_1_1,
    HTTP_2_0,
} HTTP_VERSIONS;

typedef enum StatusCode {
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
} StatusCode;

#define STATUS_CODE_COUNT 63
void *StatusCodeDef[][2] = {
    {(void *)CONTINUEE,                         "Continue" },
    {(void *)SWITCH_PROTOCOL,                   "Switching Protocols" },
    {(void *)PROCESSING,                        "Processing" },
    {(void *)EARLY_HINT,                        "Early Hints" },
    {(void *)OK,                                "OK" },
    {(void *)CREATED,                           "Created" },
    {(void *)ACCEPTED,                          "Accepted" },
    {(void *)NON_AUTHORIZED_INFO,               "Non-Authoritative Information" },
    {(void *)NO_CONTENT,                        "No Content" },
    {(void *)RESET_CONTENT,                     "Reset Content" },
    {(void *)PARTIAL_CONTENT,                   "Partial Content" },
    {(void *)MULTI_STATUS,                      "Multi-Status" },
    {(void *)ALREADY_REPRORTED,                 "Already Reported" },
    {(void *)IM_USED,                           "IM Used" },
    {(void *)MULTIPLE_CHOICES,                  "Multiple Choices" },
    {(void *)MOVED_PERMANENTLY,                 "Moved Permanently" },
    {(void *)FOUND,                             "Found" },
    {(void *)SEE_OTHER,                         "See Other" },
    {(void *)NOT_MODIFIED,                      "Not Modified" },
    {(void *)USE_PROXY,                         "Use Proxy" },
    {(void *)SWITCH_PROXY,                      "Switch Proxy" },
    {(void *)TEMP_REDIRECT,                     "Temporary Redirect" },
    {(void *)PERM_REDIRECT,                     "Permanent Redirect" },
    {(void *)BAD_REQUEST,                       "Bad Request" },
    {(void *)UNAUTHORIZED,                      "Unauthorized" },
    {(void *)PAYMENT_REQUIRED,                  "Payment Required" },
    {(void *)FORBIDDEN,                         "Forbidden" },
    {(void *)NOT_FOUND,                         "Not Found" },
    {(void *)METHOD_NOT_ALLOWED,                "Method Not Allowed" },
    {(void *)NOT_ACCEPTABLE,                    "Not Acceptable" },
    {(void *)PROXY_AUTH_REQUIRED,               "Proxy Authentication Required" },
    {(void *)REQUIRE_TIMEOUT,                   "Request Timeout" },
    {(void *)CONFLICT,                          "Conflict" },
    {(void *)GONE,                              "Gone" },
    {(void *)LENGTH_REQUIRED,                   "Length Required" },
    {(void *)PRECONDITION_FAILED,               "Precondition Failed" },
    {(void *)PAYLOAD_TOO_LARGE,                 "Content Too Large" },
    {(void *)URI_TOO_LONG,                      "URI Too Long" },
    {(void *)UNSUPPORTED_MEDIA_TYPE,            "Unsupported Media Type" },
    {(void *)RANGE_NOT_SATISFIABLE,             "Range Not Satisfiable" },
    {(void *)EXPECTATION_FAILED,                "Expectation Failed" },
    {(void *)IM_A_TEAPOT,                       "I'm a teapot" },
    {(void *)MISDIRECTED_REQUEST,               "Misdirected Request" },
    {(void *)UNPROCESSABLE_ENTITY,              "Unprocessable Content" },
    {(void *)LOCKED,                            "Locked" },
    {(void *)FAILED_DEPENDENCY,                 "Failed Dependency" },
    {(void *)TOO_EARLY,                         "Too Early" },
    {(void *)UPGRADE_REQUIRED,                  "Upgrade Required" },
    {(void *)PROCONDITION_REQUIRED,             "Precondition Required" },
    {(void *)TOO_MANY_REQUEST,                  "Too Many Requests" },
    {(void *)REQ_HEADER_FIELD_TOO_LARGE,        "Request Header Fields Too Large" },
    {(void *)UNAVAILABLE_FOR_LEGAL_REASON,      "Unavailable For Legal Reasons" },

    {(void *)INTERNAL_SERVER_ERR,               "Internal Server Error" },
    {(void *)NOT_IMPLEMENTED,                   "Not Implemented" },
    {(void *)BAD_GATEWAY,                       "Bad Gateway" },
    {(void *)SERVER_UNAVAILABLE,                "Service Unavailable" },
    {(void *)GATEWAY_TIMEOUT,                   "Gateway Timeout" },
    {(void *)HTTP_VERSION_NOT_SUPPORTED,        "HTTP Version Not Supported" },
    {(void *)VARIANT_ALSO_NEGOTIATES,           "Variant Also Negotiates" },
    {(void *)INSUFFICIENT_STORAGE,              "Insufficient Storage" },
    {(void *)LOOP_DETECTED,                     "Loop Detected" },
    {(void *)NOT_EXTENDED,                      "Not Extended" },
    {(void *)NETWORK_AUTHENTICATION_REQUIRED,   "Network Authentication Required" },
    NULL
};

StatusCode find_code_t(string c0de)
{
	for(int i = 0; i < STATUS_CODE_COUNT; i++)
	{
		void **n = (void **)StatusCodeDef[i];
		if(str_cmp(n[1], c0de))
			return (StatusCode)n[0];
	}

	return -1;
}

string code_2_str(StatusCode c0de)
{
	for(int i = 0; i < STATUS_CODE_COUNT; i++)
	{
		void **n = (void **)StatusCodeDef[i];
		if((StatusCode)n[0] == c0de)
			return n[1];
	}

	return NULL;
}

#define ENCODED_SYMBOL_COUNT 9
void *EncodedSymbols[][2] = {
    {(void *)' ',      "+"},
    {(void *)'@',      "%40"},
    {(void *)';',      "%3B"},
    {(void *)':',      "%3A"},
    {(void *)'#',      "%23"},
    {(void *)'"',      "\\"},
    {(void *)'/',      "%2F"},
    {(void *)'?',      "%3F"},
    {(void *)'=',      "%3D"},
    NULL
};

string HTTP_DATA = "HTTP/1.1 200 OK\r\n"
					"Content-type: html\r\n"
					"Content-length: 13\r\n\r\nHello World\r\n";

typedef struct {
	string 	req_type;
	string 	route;
	string 	req_version;
	map_t 	headers;	// Header(s) map
	map_t 	cookies;	// Cookie(s) map
	map_t 	GET;		// GET Params
	map_t 	POST;		// POST Params/Data

	string 	body;		// Request Body (Holding POST data)
	sArr 	lines;
} cwr_t;

typedef struct {
	string name;
	string route;
	// req_t type; 		// Acceptable request types
	string content; 	// Loaded Content - Editable
} route_t;

typedef route_t **router_t;

typedef struct {
	sock_t 		con;
	router_t 	router;
} cws_t;

cwr_t parse_request(string req, int sz)
{
	cwr_t r, empty;
	memzero(&r, sizeof(cwr_t));

	if(!req)
		return empty;

	i32 argc = 0;
	r.lines = split_string(req, '\n', &argc);
	if(argc <= 1 || !r.lines)
		return empty;

	_printf("Lines: %d\n", (ptr)&argc);
	i32 arg_c = 0;
	sArr args = split_string(r.lines[0], ' ', &arg_c);
	if(arg_c < 3 || !args)
		return empty;

	print_args((string []){"Request: ", args[0], " ", args[1], " ", args[2], "\n", NULL});
	pfree_array((array)args);

	r.body = allocate(0, 4096);
	r.headers = init_map();
	int stop_headers = 0;
	int i = 0;
	for(i = 1; i < argc; i++)
	{
		string line = r.lines[i];
		if(!line) break;

		if(line[1] == '\0') {
			stop_headers = 1;
			continue;
		}

		if(!stop_headers)
		{
			i32 pos = find_char(line, ':');
			if(pos == -1)
				continue;

			string key = line;
			line[pos] ='\0';

			string value = line + pos + 2;
			map_append(r.headers, key, value);
			continue;
		}

		str_append(r.body, line);
	}

	_printf("Body: \n%s\n", r.body);
	if(r.headers->len > 0)
		return r;

	return empty;
}

void send_response(sock_t sock, StatusCode c0de, map_t headers, map_t cookies, string body)
{
	string response = allocate(0, 4096);
	str_append(response, "HTTP/1.1 ");

	str_append_int(response, c0de);
	str_append(response, " ");
	str_append(response, code_2_str(c0de));
	str_append(response, "\r\n");

	if(headers->len > 0)
	{
		for(int i = 0; i < headers->len; i++)
		{
			str_append(response, headers->fields[i]->key);
			str_append(response, ": ");
			str_append(response, headers->fields[i]->value);
			str_append(response, "\r\n");
		}

		str_append(response, "\r\n");
	}

	if(body)
	{
		str_append(response, body);
		str_append(response, "\r\n");
	}

	if(__syscall__(sock->fd, (long)response, _str_len(response), -1, -1, -1, _SYS_WRITE) <= 0)
		println("Error, Failed to send data to client request!");
}

void req_destruct(cwr_t *r)
{
	if(r->body)
		_pfree(r->body);

	if(r->headers)
		map_destruct(r->headers);

//	if(r->lines)
//		pfree_array((array)r->lines);
}

public int entry()
{
	uninit_mem();
	set_heap_sz(_HEAP_PAGE_ * 40);
	init_mem();
	_printf("Heap Size: %d\n", (ptr)&_HEAP_PAGE_);

	sock_t n = listen_tcp(NULL, 80, 999);
	if(n->fd <= 0)
		fsl_panic("Unable to create socket...");

	println("Started");
	while(1)
	{
		sock_t client = sock_accept(n, 1024);

		string buff = sock_read(client);
		if(!buff)
			continue;

		int bytes = __get_size__(buff);
		_printf("Size: %d\n", (ptr)&bytes);

		cwr_t r = parse_request(buff, bytes);
		if(!r.headers) {
			_pfree(buff);
			continue;
		}

		map_t m = init_map();
		map_append(m, "Content-type", "text/html");
		map_append(m, "Content-length", "13");
		send_response(client, OK, m, NULL, "Hello World");
		req_destruct(&r);

		pfree(buff, 1);
		sock_close(client);
	}
	return 0;
}
