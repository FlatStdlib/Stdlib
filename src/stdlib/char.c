#include "../../headers/clibp.h"

i32 is_ascii(char c)
{
	return (unsigned char)c <= 127;
}

i32 is_ascii_alpha(char c)
{
	unsigned char b = (unsigned char)c;
	return (c > 'a' && c < 'z' || c > 'A' && c < 'Z');
}

i32 count_char(const string buffer, const char ch)
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

i32 find_char(const string buffer, const char ch)
{
	return find_char_at(buffer, ch, 1);
}

i32 find_char_at(const string buffer, const char ch, i32 match)
{
	if(!buffer)
		return -1;

	int i, count = 0;
	for(i = 0; buffer[i] != '\0'; i++)
	{
		if(buffer[i] == ch)
			count++;

		if(count == match)
			return i;
	}

	return -1;
}
