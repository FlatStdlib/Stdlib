#include "../../headers/clibp.h"

fn ptr_to_str(ptr p, string out)
{
	static const char hex[] = "0123456789abcdef";
	uintptr_t v = (uintptr_t)p;

	out[0] = '0';
	out[1] = 'x';

	int idx = 2;
	for (int i = sizeof(uintptr_t) * 2 - 1; i >= 0; i--) {
		out[idx++] = hex[(v >> (i * 4)) & 0xF]; // it uses incremental idx instead of calculating the inverse position. = speed gain.
	}
	out[idx] = '\0';
}

string int_to_str(int num)
{
	char buff[12]; // int32 + '\0'
	int i = 0;
	bool neg = false;

	if (num == 0) {
		buff[0] = '0';
		buff[1] = '\0';
		return str_dup(buff);
	}

	if (num < 0) {
		neg = true;
		num = -num;
	}

	while (num) {
		buff[i++] = '0' + (num % 10);
		num /= 10;
	}

	if (neg) buff[i++] = '-';

	// reverse
	for (int j = 0; j < i / 2; j++) {
		char t = buff[j];
		buff[j] = buff[i - j - 1];
		buff[i - j - 1] = t;
	}

	buff[i] = '\0';
	return str_dup(buff);
}

fn _sprintf(string buffer, string format, any* args)
{
	int arg = 0, idx = 0;

	for (int i = 0; format[i] != '\0'; i++)
	{
		if (format[i] == '%' && format[i + 1] == 's')
		{
			char* s = ((char**)args)[arg];
			if (!s) s = "(null)";

			for (int c = 0; s[c] != '\0'; c++)
				buffer[idx++] = s[c];

			arg++;
			i++;
			continue;
		}
		else if (format[i] == '%' && format[i + 1] == 'd')
		{
			int t = *(int*)args[arg];
			string num = int_to_str(t);

			for (int c = 0; num[c] != '\0'; c++)
				buffer[idx++] = num[c];

			pfree(num, 1);
			arg++;
			i++;
			continue;
		}
		else if (format[i] == '%' && format[i + 1] == 'p')
		{
			char ptr_buff[2 + sizeof(uintptr_t) * 2 + 1];
			ptr_to_str(args[arg], ptr_buff);

			for (int c = 0; ptr_buff[c] != '\0'; c++)
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
	char BUFF[500] = { 0 };
	while (temp)
	{
		BUFF[c++] = '0' + (temp % 10);
		temp /= 10;
	}

	for (int i = 0; i < c; i++)
	{
		char t = BUFF[i], n = BUFF[--c];
		BUFF[i] = n;
		BUFF[c] = t;
	}

	for (int i = 0; BUFF[i] != '\0'; i++) {
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
	if (!buffer)
		return 0;

	len_t count = 0;
	for (len_t i = 0; buffer[i] != '\0'; i++)
		count++;

	return count;
}

//this here work for any size
bool str_cmp(const string src, const string needle)
{
	if (!src || !needle) return false;

	for (int i = 0;; i++) {
		if (src[i] != needle[i]) return false;
		if (src[i] == '\0') break;
	}

	return true;
}

//loop with `i < len` is bad, it overwrites data.
int str_append(string buff, const string sub) {
	if (!buff || !sub) return 0;

	int idx = str_len(buff);
	for (int i = 0; sub[i] != '\0'; i++)
		buff[idx++] = sub[i];

	buff[idx] = '\0';
	return 1;
}

//Returns 1 or 0, not the actual position = bug
pos_t find_string(const string buff, const string needle) {
	if (!buff || !needle) return -1;

	int len = str_len(buff);
	int nlen = str_len(needle);

	for (int i = 0; i <= len - nlen; i++) {
		int j;
		for (j = 0; j < nlen; j++)
			if (buff[i + j] != needle[j]) break;
		if (j == nlen) return i;
	}

	return -1;
}

sArr split_lines(const string buffer, int* idx)
{
	return split_string(buffer, '\n', idx);
}

sArr split_string(const string buffer, const char ch, int* idx)
{
	if (!buffer)
		return NULL;

	i32 len = str_len(buffer);
	i32 lines = count_char(buffer, '\n');
	sArr arr = allocate(sizeof(string), lines + 1);
	*idx = 0;
	int _len = 0;

	char LINE[len];
	for (int i = 0; i < len; i++)
	{
		if (buffer[i] == '\0')
			break;

		if (buffer[i] == ch)
		{
			int n = str_len(LINE);
			if (n == 0)
			{
				LINE[0] = ' ';
				LINE[1] = '\0';
			}

			arr[(*idx)++] = str_dup(LINE);

			sArr new_arr = to_heap(arr, sizeof(string) * ((*idx) + 1));
			pfree(arr, 1);
			arr = new_arr;
			if (!arr) println("ERR\n");
			arr[*idx] = NULL;
			LINE[0] = '\0';
			_len = 0;
			continue;
		}

		LINE[_len++] = buffer[i];
		LINE[_len] = '\0';
	}

	if (*idx > 0)
		return arr;

	pfree(arr, 1);
	return NULL;
}