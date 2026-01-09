#include <clibp.h>

/* char */
int find_char_at(const string buffer, const char ch, int match)
{
	if(!buffer)
		return -1;

	int count = 0;
	for(int i = 0; buffer[i] != '\0'; i++)
	{
		if(buffer[i] == ch)
			count++;

		if(count == match)
			return i;
	}

	return -1;
}

int _find_char(const string buffer, const char ch)
{
	return find_char_at(buffer, ch, 1);
}

int _find_char_at(const string buffer, const char ch, int match, int *start)
{
	if(!buffer)
		return 1;

	int count = 0;
	for(;buffer[*start] != '\0'; (*start)++)
	{
		if(buffer[*start] == ch)
			count++;

		if(count == match)
			return *start;
	}

	return -1;
}

string get_sub_str(const string buffer, int start, int end)
{
	int len = end - start;
	string buff = allocate(0, len + 1);

	for(int i = start, idx = 0; i < len; i++, idx++)
		buff[idx] = buffer[i];

	return buff;
}

#define TEST_STR "TESTING THIS MFKR"

const string nig = "TESTING\n"
				"THIS\n"
				"SHIT\n"
				"dick\n";

int entry()
{
	__CLIBP_DEBUG__ = 1;
	char fag[] = "testing this shit nigga";
	string dick = to_heap(fag, str_len(fag));
	println(dick);

	string pussy = to_heap(fag, str_len(dick) + 5);
	println(pussy);


	string arr[100];
	int idx = 0;
	int pos = 0, match = 1, last = 0, start = 0;
	while((pos = _find_char_at(nig, '\n', match++, &start)) != -1)
	{
		arr[idx] = get_sub_str(nig, last, pos);
		last = pos + 1;

		println(arr[idx++]);
	}

	pfree(dick, 1);
	pfree(pussy, 1);
	return 0;
}
