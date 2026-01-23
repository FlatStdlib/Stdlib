#include "../headers/libweb.h"

route_t create_route(string name, string path, handler_t handle, int parse)
{
    if(!name || !path || !handle)
        return NULL;

    route_t r = allocate(0, sizeof(_route));
    r->name = str_dup(name);
    r->path = str_dup(path);
    r->handle = handle;
    r->parse_req = parse;

    return r;
}