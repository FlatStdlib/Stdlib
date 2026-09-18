#include <fsl.h>

typedef struct __attribute__((packed))
{
	i64 len;
	char data[];
} _string;

typedef _string *string_t;
#define _STRING_META_SZ_ sizeof(_string)

public i64 get_string_size(string buffer)
{ return *(i64 *)(buffer - sizeof(i64)); }

string_t get_original_string_pointer(string p)
{ return (string_t)(p - sizeof(i64)); }

public string init_string(int len)
{
	string_t p = allocate(0, sizeof(_string) + len);
	p->len = len;

	return p->data;
}

public bool increase_buffer(string buffer, i64 more)
{
	if(!buffer || !more)
		return false;

	string_t original = get_original_string_pointer(buffer);
	i64 len = get_string_size(buffer);

	string_t p = to_heap(original, len + more);
	_pfree(p);
	
	return true;
}

public string create_string(string q)
{
	int len = _str_len(q);
	string_t p = allocate(0, sizeof(_string) + len);

	p->len = len;
	mem_cpy(p->data, q, len);
	p->data[len] = '\0';
	
	return p->data;
}

public bool string_append(string *buffer, string sub)
{
	if(!buffer || !sub)
		return false;

	string_t p = get_original_string_pointer(*buffer);
	i64 len = p->len;
	i64 slen = _str_len(sub);
	i64 new_len = len + slen;
	
	string_t new_p = to_heap(p, sizeof(_string) + new_len);
	new_p->len = new_len;

	_pfree(p);
	mem_cpy(new_p->data + len, sub, slen);
	*buffer = new_p->data;
	return true;
}

public bool string_replace(string *buffer, string find, string replacement)
{
	if(!buffer || !find)
		return false;

	string_t original = get_original_string_pointer(*buffer);
	i64 len = original->len;
	i64 slen = _str_len(find);
	i64 vlen = _str_len(replacement);

	if(vlen <= slen)
	{
		for(int i = 0, idx = 0; i < len; i++)
		{
			if(mem_cmp(original->data + i, find, slen))
			{
				mem_cpy(original->data + i, replacement, vlen);
				idx += vlen;
				i += vlen - 1;
				continue;
			}

			original->data[idx++] = original->data[i];
		}

		return true;
	} else {
		int new_len = len + vlen - (vlen - slen);
		string_t new_p = to_heap(original, new_len + sizeof(i64) + 2);
		new_p->len = new_len;

		int idx = 0;
		for(int i = 0; i < len; i++)
		{
			if(mem_cmp(original->data + i, find, slen))
			{
				mem_cpy(new_p->data + idx, replacement, vlen);
				idx += vlen;
				i += vlen;
				continue;
			}

			new_p->data[idx++] = original->data[i];
		}

		new_p->data[idx] = '\0';
		_pfree(original);
		*buffer = new_p->data;
		return true;
	}

	return false;
}

public bool is_string_lowercase(string buffer)
{
	if(!buffer)
		return false;

	for(int i = 0; buffer[i] != '\0'; i++)
	{
		if(!is_lowercase(buffer[i]))
			return false;
	}

	return true;
}

public bool is_string_uppercase(string buffer)
{
	if(!buffer)
		return false;

	for(int i = 0; buffer[i] != '\0'; i++)
	{
		if(!is_uppercase(buffer[i]))
			return false;
	}

	return true;
}

#define __macros_mem_cpy(dest, src, size) 	\
	register void *rdi asm("rdi") = dest;	\
	register void *rsi asm("rsi") = src;	\
	register long rcx asm("rcx") = size;	\
	asm("1:\n\t" 							\
        "lodsb\n\t" 						\
        "stosb\n\t" 						\
        "dec %rcx\n\t" 						\
        "jnz 1b\n\t");

int entry()
{
	int old_size = used_mem;
	// toggle_debug_mode();
	string n = create_string("testing");
	println(n);

	string v = init_string(7);
	mem_cpy(v, " this", 5);

	if(!string_append(&n, v) || !string_append(&n, " string"))
		fsl_warning("failed to append to string");

	println(n);
	string_replace(&n, "string", "char ptr");
	println(n);

	int sz = get_string_size(n);
	int len = _str_len(n);

	print("Size: "), printi(sz), print(" '"), print_sz(n, sz - 1), println("'");
	print("Actual Size: "), printi(len), print(" '"), print_sz(n, sz - 1), println("'");
	print("Heap Used: "), printi(used_mem), print("/"), printi(_HEAP_PAGE_), println(NULL);
	int new_size = used_mem;
	print("Used: "), printi(new_size - old_size), println(NULL);


	string t = allocate(0, 14);
	__macros_mem_cpy(t, n, sz);

	println(t);

	int chk = is_string_lowercase(t + 9);
	if(chk)
		println("Lowercase");


	return 0;
}
