#include "../../../headers/fsl.h"

public i32 is_ascii(char c)
{ return (unsigned char)c <= 127; }

public i32 is_ascii_alpha(char c)
{
	unsigned char b = (unsigned char)c;
	return (b > 'a' && b < 'z' || b > 'A' && b < 'Z');
}

public i32 count_char(const string buffer, const char ch)
{
	if(!buffer)
		return -1;

	int i, count = 0;
	for(i = 0; buffer[i] != '\0'; i++)
	{
		if(buffer[i] == ch)
			count++;
	}

	return count;
}

public i32 find_char(const string buffer, const char ch)
{
	return find_char_at(buffer, ch, 1);
}

public i32 find_char_at(const string buffer, const char ch, i32 match)
{
	if(!buffer)
		return -1;

	int i, count = 0;
	for(i = 0; buffer[i] != '\0'; i++)
	{
		if(buffer[i] == ch)
			count++;

		if(match == count)
			return i;
	}

	return -1;
}

public int _find_char_at(const string buffer, const char ch, int match, int *start)
{
	if(!buffer)
    	return -1;

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

public int replace_char(string buffer, const char find, const char replace)
{
	if(!buffer)
		return 0;

	int count = 0;
	for(int i = 0; buffer[i] != '\0'; i++)
	{
		if(buffer[i] == find)
		{
			buffer[i] = replace;
			count++;
		}
	}

	return count;
}

public bool trim_char(string buff, int ch)
{
	if(!buff)
		return false;

	int idx = 0;
	for(int i = 0; buff[i] != '\0'; i++) {
		if(i == ch)
			continue;

		char n = buff[i];
		buff[idx++] = n;
	}

	return true;
}

public bool trim_char_idx(string buff, int pos)
{
	if(!buff)
		return false;

	int idx = 0;
	for(int i = 0; buff[i] != '\0'; i++) {
		if(i == pos)
			continue;

		char n = buff[i];
		buff[idx++] = n;
	}

	return true;
}
