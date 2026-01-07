#include "../headers/clibp.h"

typedef enum
{
	ANY_NULL 		= 0,
	ANY_INT 		= 1,
	ANY_CHAR 		= 2,
	ANY_FLOAT 		= 3,
	ANY_STRING 		= 4,
	ANY_ARRAY 		= 5,
	ANY_PTR_ARRAY 	= 6,
} any_type;

typedef struct
{
	any_type type;
	any pointer;
} any_t;

const int ANY_META_SZ = sizeof(any_t);

any any_allocate(len_t sz, len_t len)
{
	len_t num = sz ? sz * len : len;
	if(len == 0)
		return NULL;

	len += ANY_META_SZ;
	any p = allocate(0, len);
	if(!p)
		return NULL;

	return (any)((char *)p + ANY_META_SZ);
}

void *convert_to_type(any_t p)
{
//	switch(p->type)
//	{
//		case ANY_INT:
//			int n = atoi(p->pointer);
//			return n;
//	}
}

int any_cmp(any p, any v, len_t len)
{
	if(((char *)p - ANY_META_SZ - sizeof(int)) == 0x7C)
	{
		clibp_panic("[ x ] error, invalid memory provided...!\n");
	}

	any_t n = (any)((char *)p - ANY_META_SZ);
//	switch(n->type)
//	{
//		case ANY_INT:
//			int value = v
//	}
}
