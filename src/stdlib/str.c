#include "../../headers/clibp.h"

fn _sprintf(string buffer, string format, any *args)
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
            int t = *(int *)args[arg];
            string num = int_to_str(*(char *)args[arg]);
            if(t > 9)
            	for(int c = 0; num[c] != '\0'; c++)
            		buffer[idx++] = num[c];
           	else
           		buffer[idx++] = t + '0';

           	pfree(num, 1);
            arg++;
            i++;
            continue;
        } else if(format[i] == '%' && format[i + 1] == 'p') {
			char ptr_buff[100];
			ptr_to_str(args[arg], ptr_buff);

			for(int c = 0; ptr_buff[c] != '\0'; c++)
				buffer[idx++] = ptr_buff[c];

			arg++;
			i++;
			continue;
        }

        buffer[idx++] = format[i];
    }
    buffer[idx] = '\0';
}

fn istr(string dest, int num)
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

string str_dup(const string buff)
{
	int len = str_len(buff);

	string buffer = (string)allocate(0, len + 1);
	mem_cpy(buffer, buff, len);

	buffer[len - 1] = '\0';
	return buffer;
}

len_t str_len(const string buffer)
{
	if(!buffer)
		return 0;

	len_t count = 0;
	for(len_t i = 0; buffer[i] != '\0'; i++)
		count++;

	return count;
}

// String Compare
bool str_cmp(const string src, const string needle)
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
int stra(string buff, const string sub) {
	if(!buff || !sub)
		return 0;

	len_t src_len = str_len(buff);
	len_t len = str_len(sub);
	for(int i = src_len, c = 0; i < len; i++, c++) {
		buff[i] = sub[c];
	}

	return 1;
}

// Find String (to find multiple, increament match each call until -1)
pos_t find_str(const string buff, const string needle)
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
