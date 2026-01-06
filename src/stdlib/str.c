#include "../../headers/clibp.h"

fn_t _sprintf(str buffer, str format, any *args)
{
    int arg = 0, idx = 0;
    for(int i = 0; format[i] != '\0'; i++)
    {
        if(format[i] == '%' && format[i + 1] == 's')
        {
            for(int c = 0; ((char **)args)[arg][c] != '\0'; c++) {
                buffer[idx++] = ((char **)args)[arg][c];
            }
            arg++;
            i++;
            continue;
        } else if(format[i] == '%' && format[i + 1] == 'd') {
            istr(buffer + idx, *(int *)args[arg]);

            arg++;
            i++;
            continue;
        }

        buffer[idx++] = format[i];
    }
}

fn_t istr(str dest, int num)
{
	int temp = num, c = 0;
	char BUFF[500] = {0};
    while(temp)
    {
    	BUFF[c++] = '0' + (temp % 10);
		temp /= 10;
	}

    for(int i = 0; i < c; i++)
    {
    	char t = BUFF[i], n = BUFF[--c];
        BUFF[i] = n;
        BUFF[c] = t;
    }

    for(int i = 0; BUFF[i] != '\0'; i++) {
    	dest[i] = BUFF[i];
    }
}

str str_dup(const str buff)
{
	int len = str_len(buff);

	str buffer = (str)allocate(0, len);
	mem_cpy(buffer, buff, len);

	return buffer;
}

len_t str_len(const str buffer)
{
	if(!buffer)
		return 0;

	len_t count = 0;
	for(len_t i = 0; buffer[i] != '\0'; i++)
		count++;

	return count;
}

// String Compare
bool str_cmp(const str src, const str needle)
{
	if(!src || !needle)
		return false;

	len_t len = str_len(src);
	len_t len2 = str_len(needle);

	if(len2 > len)
		return false;

	for(int i = 0; i < len2; i++)
	{
		if(src[i] != needle[i])
			return false;
	}

	return true;
}

// String Append
int stra(str buff, const str sub) {
	if(!buff || !sub)
		return 0;

	len_t src_len = str_len(buff);
	len_t len = str_len(sub);
	for(int i = src_len, c = 0; i < len; i++, c++) {
		buff[i] = sub[c];
	}

	return 1;
}

// Find Char (to find multiple, increament match each call until -1 or set to 0 to just find 1)
pos_t find_char(const str buff, const char ch, int match)
{
	if(!buff || ch == 0)
		return -1;

	len_t len = str_len(buff);
	for(int i = 0; i < len; i++)
	{
		if(buff[i] == '\0')
			break;

		if(buff[i] == ch && i == match)
			return i;
	}

	return -1;
}

// Find String (to find multiple, increament match each call until -1)
pos_t find_str(const str buff, const str needle)
{
    int len = str_len(buff);
    int slen = str_len(needle);
    for(int i = 0; i < len; i++)
    {
        int chk = 0;
        for(int c = 0; c < slen; c++) {
            if(buff[i + c] != needle[c])
                return 0;
            else
                chk = 1;
        }

        if(chk)
            return 1;
    }

    return 0;
}
