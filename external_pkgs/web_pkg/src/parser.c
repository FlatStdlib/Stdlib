#include "../headers/libweb.h"

field_t DEFAULT_HEADERS[] = {
    &(_field){ .key = "Content-Type", .value = "text/html;charset=UTF-8" },
    &(_field){ .key = "Connection", .value = "close" },
    NULL
};

ptr status_code_strings[][2] = {
    {(ptr)CONTINUEE,                         "Continue" },
    {(ptr)SWITCH_PROTOCOL,                   "Switching Protocols" },
    {(ptr)PROCESSING,                        "Processing" },
    {(ptr)EARLY_HINT,                        "Early Hints" },
    {(ptr)OK,                                "OK" },
    {(ptr)CREATED,                           "Created" },
    {(ptr)ACCEPTED,                          "Accepted" },
    {(ptr)NON_AUTHORIZED_INFO,               "Non-Authoritative Information" },
    {(ptr)NO_CONTENT,                        "No Content" },
    {(ptr)RESET_CONTENT,                     "Reset Content" },
    {(ptr)PARTIAL_CONTENT,                   "Partial Content" },
    {(ptr)MULTI_STATUS,                      "Multi-Status" },
    {(ptr)ALREADY_REPRORTED,                 "Already Reported" },
    {(ptr)IM_USED,                           "IM Used" },
    {(ptr)MULTIPLE_CHOICES,                  "Multiple Choices" },
    {(ptr)MOVED_PERMANENTLY,                 "Moved Permanently" },
    {(ptr)FOUND,                             "Found" },
    {(ptr)SEE_OTHER,                         "See Other" },
    {(ptr)NOT_MODIFIED,                      "Not Modified" },
    {(ptr)USE_PROXY,                         "Use Proxy" },
    {(ptr)SWITCH_PROXY,                      "Switch Proxy" },
    {(ptr)TEMP_REDIRECT,                     "Temporary Redirect" },
    {(ptr)PERM_REDIRECT,                     "Permanent Redirect" },
    {(ptr)BAD_REQUEST,                       "Bad Request" },
    {(ptr)UNAUTHORIZED,                      "Unauthorized" },
    {(ptr)PAYMENT_REQUIRED,                  "Payment Required" },
    {(ptr)FORBIDDEN,                         "Forbidden" },
    {(ptr)NOT_FOUND,                         "Not Found" },
    {(ptr)METHOD_NOT_ALLOWED,                "Method Not Allowed" },
    {(ptr)NOT_ACCEPTABLE,                    "Not Acceptable" },
    {(ptr)PROXY_AUTH_REQUIRED,               "Proxy Authentication Required" },
    {(ptr)REQUIRE_TIMEOUT,                   "Request Timeout" },
    {(ptr)CONFLICT,                          "Conflict" },
    {(ptr)GONE,                              "Gone" },
    {(ptr)LENGTH_REQUIRED,                   "Length Required" },
    {(ptr)PRECONDITION_FAILED,               "Precondition Failed" },
    {(ptr)PAYLOAD_TOO_LARGE,                 "Content Too Large" },
    {(ptr)URI_TOO_LONG,                      "URI Too Long" },
    {(ptr)UNSUPPORTED_MEDIA_TYPE,            "Unsupported Media Type" },
    {(ptr)RANGE_NOT_SATISFIABLE,             "Range Not Satisfiable" },
    {(ptr)EXPECTATION_FAILED,                "Expectation Failed" },
    {(ptr)IM_A_TEAPOT,                       "I'm a teapot" },
    {(ptr)MISDIRECTED_REQUEST,               "Misdirected Request" },
    {(ptr)UNPROCESSABLE_ENTITY,              "Unprocessable Content" },
    {(ptr)LOCKED,                            "Locked" },
    {(ptr)FAILED_DEPENDENCY,                 "Failed Dependency" },
    {(ptr)TOO_EARLY,                         "Too Early" },
    {(ptr)UPGRADE_REQUIRED,                  "Upgrade Required" },
    {(ptr)PROCONDITION_REQUIRED,             "Precondition Required" },
    {(ptr)TOO_MANY_REQUEST,                  "Too Many Requests" },
    {(ptr)REQ_HEADER_FIELD_TOO_LARGE,        "Request Header Fields Too Large" },
    {(ptr)UNAVAILABLE_FOR_LEGAL_REASON,      "Unavailable For Legal Reasons" },

    {(ptr)INTERNAL_SERVER_ERR,               "Internal Server Error" },
    {(ptr)NOT_IMPLEMENTED,                   "Not Implemented" },
    {(ptr)BAD_GATEWAY,                       "Bad Gateway" },
    {(ptr)SERVER_UNAVAILABLE,                "Service Unavailable" },
    {(ptr)GATEWAY_TIMEOUT,                   "Gateway Timeout" },
    {(ptr)HTTP_VERSION_NOT_SUPPORTED,        "HTTP Version Not Supported" },
    {(ptr)VARIANT_ALSO_NEGOTIATES,           "Variant Also Negotiates" },
    {(ptr)INSUFFICIENT_STORAGE,              "Insufficient Storage" },
    {(ptr)LOOP_DETECTED,                     "Loop Detected" },
    {(ptr)NOT_EXTENDED,                      "Not Extended" },
    {(ptr)NETWORK_AUTHENTICATION_REQUIRED,   "Network Authentication Required" },
    NULL
};

handler_t request_handler(cwr_t wr)
{
	wr->content = sock_read(wr->socket);
	int len = __get_meta__(wr->content)->length;

	if(len < 3)
	{
		println("Malform request...!\n");
		return NULL;
	}

	/* A Web request involves arguments by space even with lines so */
	if(find_char(wr->content, ' ') == -1)
		return NULL;

	wr->lines = split_lines(wr->content, &wr->line_count);
	if(wr->line_count == 0)
	{
		println("Malform request\n");
		return NULL;
	}

	int c = 0;
	sArr info = split_string(wr->lines[0], ' ', &c);
	if(find_string(info[0], "GET")) {
		wr->type = _rGET;
	} else if(find_string(info[0], "POST"))
	{
		wr->type = _rPOST;
	} else if(find_string(info[0], "HEAD"))
	{
		wr->type = _rHEAD;
	}

	// Trim GET parameters from URI
	wr->uri = str_dup(info[1]);
	int r = find_route(_WEB_, info[1]);
	if(r == -1) {
		print_args((string []){"[ WEB_SERVER ]: Attempt @ ", info[1], "\n", 0});
		return NULL;
	}
	
		print_args((string []){"[ WEB_SERVER ]: New request @ ", info[1], "\n", 0});

	if(__CLIBP_DEBUG__)
		print_args((string []){"Triggering: ", _WEB_->routes[r]->name, "\n", 0});

	if(_WEB_->routes[r]->parse_req) {
		parse_request(wr);
	}

	_WEB_->routes[r]->handle(_WEB_->routes[r], wr);

	request_Destruct(wr);
	return NULL;
}

i32 parse_get_params(cwr_t wr)
{
	if(!wr->uri)
		return -1; // No URI set

	int pos = find_char(wr->uri, '/');
	if(pos == -1)
		return -1; // No params found

	string params = wr->uri + (pos + 1);
	wr->get = init_map();
	if(find_char(params, '&') > -1)
	{
		int argc = 0;
		sArr argv = split_string(params, '&', &argc);
		if(!argc)
			return -1;

		for(int i = 0; i < argc; i++)
		{
			int arg_c = 0;
			sArr args = split_string(argv[i], '=', &arg_c);
			if(!args)
				continue;

			if(arg_c < 2)
				continue;
			
			map_append(wr->get, args[0], args[1]);
		}
	}

	int argc = 0;
	sArr args = split_string(params, '=', &argc);
	if(!args)
		return -1;

	if(argc < 2)
	{
		pfree_array((array)args);
		return -1;
	}

	map_append(wr->get, args[0], args[1]);
	return wr->get->len;
}

fn parse_request(cwr_t wr)
{
	bool capture_body = false;

	int len = __get_meta__(wr->content)->length;
	wr->body = allocate(0, len);

	/* Parse Headers and Get body, Skip first line */
	wr->headers = init_map();
	for(int i = 1; i < wr->line_count; i++)
	{
		if(!wr->lines[i]) {
			break;
		}

		/* After headers which is an empty line, Capture Body */
		if(is_empty(wr->lines[0])) {
			capture_body = true;
			continue;
		} else if(is_empty(wr->lines[0]) && capture_body) { // Break after body
			break;
		}

		/* Capture body after header */
		if(capture_body)
		{
			str_append(wr->body, wr->lines[i]);
			continue;
		}

		/* Parse Header Line */
		if(find_char(wr->lines[i], ':') > -1)
		{
			int arg_c = 0;
			sArr args = split_string(wr->lines[i], ':', &arg_c);

			if(!args) {
				continue;
			} else if(arg_c < 2)
			{
				clibp_panic("unable to get header arguments!");
				// TODO; CHANGE, THIS ACTUALLY DOESNT FREE THE ELEMENTS
				pfree((array)args, 1);
				continue;
			}

			int pos = find_char(wr->lines[i], ':');
			if(!map_append(wr->headers, args[0], wr->lines[i] + (pos + 2)))
				clibp_panic("failed to get header");
			
			// TODO; CHANGE, THIS ACTUALLY DOESNT FREE THE ELEMENTS
			pfree((array)args, 1);
		}
	}
}

fn parse_post(cwr_t wr)
{
	if(!wr)
		return;

	if(find_char(wr->body, '=') == -1)
		return;

	wr->post = init_map();
	if(find_char(wr->body, '&') > -1)
	{
		int arg_c = 0;
		sArr args = split_string(wr->body, '&', &arg_c);
		if(!arg_c)
			return;

		for(int i = 0; i < arg_c; i++)
		{
			int argc = 0;
			sArr arg = split_string(args[i], '=', &argc);
			if(!arg || !argc)
				continue;

			map_append(wr->post, arg[0], arg[1]);
		}

		pfree_array((void *)args);
	}

	int argc = 0;
	sArr args = split_string(wr->body, '=', &argc);
	if(!args || !argc)
		return;

	map_append(wr->post, args[0], args[1]);
	pfree_array((void *)args);
}

fn parse_get_parameters(cwr_t wr)
{
	if(!wr)
		return;

	int pos = find_char(wr->uri, '?');
	if(pos == -1)
		return;

	wr->get = init_map();
	string parameters = wr->body + pos;
	if(find_char(parameters, '&')) {
		int argc = 0;
		sArr params = split_string(parameters, '&', &argc);
		if(!argc || !params)
			return;

		for(int i = 0; i < argc; i++)
		{
			int arg_c = 0;
			sArr field = split_string(params[i], '=', &arg_c);
			if(!arg_c || !field)
				continue;

			map_append(wr->get, field[0], field[1]);
			pfree_array((void *)field);
		}
	}

}

fn request_Destruct(cwr_t wr)
{
	if(wr->socket)
		sock_close(wr->socket);

	if(wr->headers)
		map_destruct(wr->headers);
	
	if(wr->path)
		pfree(wr->path, 1);

	if(wr->uri)
		pfree(wr->uri, 1);

	if(wr->http_version)
		pfree(wr->http_version, 1);

	if(wr->post)
		pfree(wr->post, 1);

	if(wr->get)
		pfree(wr->get, 1);

	if(wr->content)
		pfree(wr->content, 1);

	if(wr->body, 1);

	if(wr->lines);
		pfree_array((array)wr->lines);

	// TODO; create a thread destructor in its lib

	pfree(wr, 1);
}

fn send_response(cwr_t wr, _response r, bool heap_content)
{
    // TODO; change this hardcoded size shit
    string ctx = allocate(0, 4096);

    str_append(ctx, "HTTP/1.1 ");
    str_append_int(ctx, r.code);
    str_append(ctx, " ");
    str_append(ctx, status_code_to_string(r.code));
    str_append(ctx, "\r\n");

    int body_len = 1;
	if(heap_content)
		body_len = __get_size__(r.content) + 3;
	else
		body_len = str_len(r.content) + 3;

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
    if(r.content)
	{
		str_append(ctx, r.content);
		str_append(ctx, "\r\n\r\n");
	}

    sock_write(wr->socket, ctx);
    if(__CLIBP_DEBUG__)
        print("Generated Response: "), println(ctx);

    pfree(ctx, 1);
}
