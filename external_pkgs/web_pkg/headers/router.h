#pragma once

#include <clibp.h>

#ifndef _LIBWEB_ROUTER_H
    #define _LIBWEB_ROUTER_H
    typedef struct
    {
        string      name;
        string      path;

        string      template;
        handler_t   handle;
        /*
            For Non View-Only sites, These must be enabled 
        */
        bool        parse_req;
    } _route;

    typedef _route *route_t;
    typedef _route 	**rArr;

    route_t create_route(string name, string path, handler_t fnc, int view_only);
    bool get_html_template(route_t r, string template_file);
    bool toggle_read_only(route_t r);
#endif