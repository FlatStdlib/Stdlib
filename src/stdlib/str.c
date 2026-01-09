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

	buffer[len] = '\0';
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

	if(len2 > len || len2 < len)
		return false;

	for(int i = 0; i < len; i++)
	{
		if(src[i] != needle[i])
			return false;
	}

	return true;
}

// String Append
int str_append(const string buff, const string sub) {
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
pos_t find_string(const string buff, const string needle)
{
	if(!buff || needle)
    	return -1;

    int len = str_len(buff);
    int slen = str_len(needle);
    for(int i = 0; i < len; i++)
    {
        int chk = 0;
        for(int c = 0; c < slen; c++) {
            if(buff[i + c] != needle[c])
				break;
            else
                chk = 1;
        }

        if(chk)
            return 1;
    }

    return 0;
}

sArr split_lines(const string buffer, int *idx)
{
     return split_string(buffer, '\n', idx);
}

sArr split_string(const string buffer, const char ch, int *idx)
{
    if(!buffer)
        return NULL;

    i32 len = str_len(buffer);
        i32 lines = count_char(buffer, '\n');
    sArr arr = allocate(sizeof(string), lines + 1);
    *idx = 0;
    int _len = 0;

    char LINE[len];
    for(int i = 0; i < len; i++)
    {
        if(buffer[i] == '\0')
            break;

        if(buffer[i] == ch)
        {
            int n = str_len(LINE);
            if(n == 0)
            {
                LINE[0] = ' ';
                LINE[1] = '\0';
            }

            arr[(*idx)++] = str_dup(LINE);

            sArr new_arr = to_heap(arr, sizeof(string) * ((*idx) + 1));
            pfree(arr, 1);
            arr = new_arr;
            if(!arr) println("ERR\n");
            arr[*idx] = NULL;
            LINE[0] = '\0';
            _len = 0;
            continue;
        }

        LINE[_len++] = buffer[i];
        LINE[_len] = '\0';
    }

    if(*idx > 0)
        return arr;

    pfree(arr, 1);
    return NULL;
}
