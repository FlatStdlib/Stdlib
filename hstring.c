#include <fsl.h>

typedef i8 *string_t;
#define __STRING_METADATA_SZ__ sizeof(i64)

public i64 get_string_size(string_t buffer)
{ return *(i64 *)(buffer - __STRING_METADATA_SZ__ - 1); }

public string_t get_original_string_pointer(string_t buffer)
{ return (buffer - __STRING_METADATA_SZ__ - 1); }

public string_t init_string(int len)
{
	string_t p = allocate(0, __STRING_METADATA_SZ__ + len + 2);
	*((i64 *)p) = len;

	return p + __STRING_METADATA_SZ__ + 1;
}

public bool increase_buffer(string_t *buffer, i64 more)
{
	if(!buffer || !more)
		return false;

	i64 len = get_string_size(*buffer);
	string_t p = to_heap(get_original_string_pointer(*buffer), len + more);

	*buffer = p;
	return true;
}

public string_t create_string(string q)
{
	i64 len = _str_len(q);
	string_t p = (i8 *)allocate(0, __STRING_METADATA_SZ__ + len + 2);
	*((i64 *)p) = len;

	mem_cpy(p + __STRING_METADATA_SZ__ + 1, q, len);
	return p + __STRING_METADATA_SZ__ + 1;
}

public bool string_append(string_t *buffer, string sub)
{
	if(!buffer || !sub)
		return false;

	string_t original_p = get_original_string_pointer(*buffer);
	i64 len = *(i64 *)original_p;
	i64 slen = str_len(sub);

	i64 new_len = len + slen;
	string_t new_p = to_heap(original_p, new_len + __STRING_METADATA_SZ__ + 1);
	*((i64 *)new_p) = new_len;

	*buffer = new_p + __STRING_METADATA_SZ__ + 1;

	mem_cpy((*buffer) + len, sub, slen);
	_pfree(original_p);
	return true;
}

public bool string_replace(string_t *buffer, string find, string replacement)
{
	if(!buffer || !find)
		return false;

	string_t original_p = get_original_string_pointer(*buffer);
	i64 len = *(i64 *)original_p;
	i64 slen = _str_len(find);
	i64 vlen = _str_len(replacement);

	if(vlen <= slen)
	{
		for(int i = 0, idx = 0; i < len; i++)
		{
			if(mem_cmp((*buffer) + i, find, slen))
			{
				mem_cpy((*buffer) + i, replacement, vlen);
				idx += vlen;
				i += vlen - 1;
				continue;
			}

			(*buffer)[idx++] = (*buffer)[i];
		}

		return true;
	} else {
		int new_len = len + vlen - (vlen - slen);
		string_t new_p = to_heap(original_p, new_len + sizeof(i64) + 2);
		*((i64 *)new_p) = new_len;

		string_t new_buff = new_p + __STRING_METADATA_SZ__ + 1;
		int idx = 0;
		for(int i = 0; i < len; i++)
		{
			if(mem_cmp((*buffer) + i, find, slen))
			{
				mem_cpy(new_buff + idx, replacement, vlen);
				idx += vlen;
				i += vlen;
				continue;
			}

			new_buff[idx++] = (*buffer)[i];
		}

		new_buff[idx] = '\0';
		_pfree(original_p);
		*buffer = new_buff;
		return true;
	}

	return false;
}

public bool is_string_lowercase(string_t buffer)
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

public bool is_string_uppercase(string_t buffer)
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
	string_t n = create_string("testing");
	println(n);

	string v = init_string(7);
	mem_cpy(v, " this", 5);

	if(!string_append(&n, v) || !string_append(&n, " string"))
		fsl_warning("failed to append to string");

	println(n);
	string_replace(&n, "string", "char ptr");

	int sz = get_string_size(n);
	int len = _str_len(n);

	print("Size: "), printi(sz), print(" '"), print_sz(n, sz), println("'");
	print("Actual Size: "), printi(len), print(" '"), print_sz(n, sz), println("'");
	print("Heap Used: "), printi(used_mem), print("/"), printi(_HEAP_PAGE_), println(NULL);
	int new_size = used_mem;
	print("Used: "), printi(new_size - old_size), println(NULL);


	string_t t = allocate(0, 14);
	__macros_mem_cpy(t, n, sz);

	println(t);

	int chk = is_string_lowercase(t + 9);
	if(chk)
		println("Lowercase");

	return 0;
}
