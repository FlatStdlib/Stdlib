#include "../../headers/clibp.h"

// substituted.
string int_to_str(int value)
{
  static char buffer[32];
  char temp[32];
  int idx = 0;
  bool negative = false;

  if (value < 0)
  {
    negative = true;
    value = -value;
  }

  if (value == 0)
    temp[idx++] = '0';
  else
  {
    while (value > 0)
    {
      temp[idx++] = '0' + (value % 10);
      value /= 10;
    }
  }

  if (negative)
    temp[idx++] = '-';

  for (int i = 0; i < idx; i++)
    buffer[i] = temp[idx - i - 1];
  buffer[idx] = '\0';

  return buffer;
}

// moved(internal.c -> str.c)
fn ptr_to_str(ptr p, string out)
{
  static const char hex[] = "0123456789abcdef";
  uintptr_t v = (uintptr_t)p;

  out[0] = '0';
  out[1] = 'x';

  for (int i = (sizeof(uintptr_t) * 2) - 1; i >= 0; i--)
  {
    out[2 + ((sizeof(uintptr_t) * 2 - 1) - i)] =
        hex[(v >> (i * 4)) & 0xF];
  }

  out[2 + sizeof(uintptr_t) * 2] = '\0';
}

fn _sprintf(string buffer, string format, any *args)
{
  int arg = 0, idx = 0;

  for (int i = 0; format[i] != '\0'; i++)
  {
    if (format[i] == '%' && format[i + 1] == 's')
    {
      char *str = ((char **)args)[arg++];
      for (int c = 0; str[c] != '\0'; c++)
        buffer[idx++] = str[c];
      i++;
    }
    else if (format[i] == '%' && format[i + 1] == 'd')
    {
      int t = *(int *)args[arg++];
      char num[32];
      int nlen = sprintf(num, "%d", t);
      for (int c = 0; c < nlen; c++)
        buffer[idx++] = num[c];
      i++;
    }
    else if (format[i] == '%' && format[i + 1] == 'p')
    {
      void *ptr = args[arg++];
      char ptr_buff[32];
      sprintf(ptr_buff, "%p", ptr);
      for (int c = 0; ptr_buff[c] != '\0'; c++)
        buffer[idx++] = ptr_buff[c];
      i++;
    }
    else
    {
      buffer[idx++] = format[i];
    }
  }

  buffer[idx] = '\0';
}

fn istr(string dest, int num)
{
  sprintf(dest, "%d", num);
}

string str_dup(const string buff)
{
  if (!buff)
    return NULL;

  len_t len = str_len(buff);
  string copy = (string)allocate(0, len + 1);
  memcpy(copy, buff, len);
  copy[len] = '\0';
  return copy;
}

len_t str_len(const string buffer)
{
  if (!buffer)
    return 0;
  const char *ptr = buffer;
  while (*ptr)
    ptr++;
  return ptr - buffer;
}

// String Compare
bool str_cmp(const string src, const string needle)
{
  if (!src || !needle)
    return false;

  len_t len1 = str_len(src);
  len_t len2 = str_len(needle);

  if (len1 != len2)
    return false;
  return memcmp(src, needle, len1) == 0;
}

int str_append(const string buff, const string sub)
{
  if (!buff || !sub)
    return 0;

  len_t len1 = str_len(buff);
  len_t len2 = str_len(sub);

  memcpy(buff + len1, sub, len2);
  buff[len1 + len2] = '\0';

  return 1;
}

// Find Char (to find multiple, increament match each call until -1 or set to 0 to just find 1)
pos_t find_char(const string buff, const char ch, int match)
{
  if (!buff || !needle)
    return -1;

  len_t len = str_len(buff);
  len_t slen = str_len(needle);

  if (slen == 0 || slen > len)
    return -1;

  for (len_t i = 0; i <= len - slen; i++)
  {
    if (memcmp(buff + i, needle, slen) == 0)
      return i;
  }

  return -1;
}

sArr split_lines(const string buffer, int *idx)
{
  return split_string(buffer, '\n', idx);
}

sArr split_string(const string buffer, const char ch, int *idx)
{
  if (!buffer)
    return NULL;

  len_t len = str_len(buffer);
  int count = 0, cap = 4;

  sArr arr = allocate(sizeof(string), cap);
  size_t start = 0;

  for (len_t i = 0; i <= len; i++)
  {
    if (buffer[i] == ch || buffer[i] == '\0')
    {
      len_t slen = i - start;
      char tmp[slen + 1];
      if (slen == 0)
      {
        tmp[0] = ' ';
        tmp[1] = '\0';
      }
      else
      {
        memcpy(tmp, buffer + start, slen);
        tmp[slen] = '\0';
      }

      if (count >= cap)
      {
        cap *= 2;
        arr = to_heap(arr, sizeof(string) * cap);
      }

      arr[count++] = str_dup(tmp);
      start = i + 1;
    }
  }

  *idx = count;
  return arr;
}
