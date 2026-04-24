#include "../../../headers/fsl.h"

public str_t init_str()
{
    str_t t     = {0};
    t.p         = allocate(0, 512);
    t.len       = 0;

    return t;
}

public int strh_len(str_t str)
{ return str.len; }

public bool strh_append(str_t str, const string sub) {
	if (!sub) return false;

	__meta__ *info = __get_meta__(str.p);
    if(info->id != 0x7C) return 0;

    len_t sub_len = str_len(sub);
    if(str.len + sub_len > info->size)
        str.p = reallocate(str.p, str.len + sub_len + 1);

	for (int i = 0; sub[i] != '\0'; i++)
		str.p[str.len++] = sub[i];

	str.p[str.len] = '\0';
	return true;
}