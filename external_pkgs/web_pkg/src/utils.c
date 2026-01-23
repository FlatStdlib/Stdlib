#include "../headers/libweb.h"

#define STATUS_CODE_COUNT 63

string status_code_to_string(status_code code)
{
    if(!code) return NULL;

    for(int i = 0; i < STATUS_CODE_COUNT; i++)
        if(code == (status_code)status_code_strings[i][0])
            return status_code_strings[i][1];

    return NULL;
}