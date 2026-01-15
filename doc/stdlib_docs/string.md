<div align="center">
    <h1>clib+ Libraries</h1>

[Start-up](/README.md)
| [Introduction](/doc/intro.md)
| ● [Libraries](/doc/libraries.md)
| [Todo](/doc/todo.md)
</div>

## List Of Libraries Provided for clib+
<table align="center">
<tr><td valign=top>

<p>Stdlib<p>

* [Internal](/doc/stdlib_docs/internal.md)
* [Allocator](/doc/stdlib_docs/allocator.md)
* [Memory](/doc/stdlib_docs/memory.md)
* [Int](/doc/stdlib_docs/int.md)
* [String](/doc/stdlib_docs/string.md)
* [Array](/doc/stdlib_docs/array.md)
* [Map](/doc/stdlib_docs/map.md)
* [File](/doc/stdlib_docs/file.md)
* [Socket](/doc/stdlib_docs/socket.md)
</td>
<td valign=top>

<p>External Libs</p>

* [cWS (C Webserver)](#)
</td>
<td valign=top>

<p>Public Libs</p>

* [GLFW2](#)
</td></tr>
</table>

# String Library

### Definitions

```c
/*
    This macros-function currently only acepts %s, %d and %p.
    It takes pointers to variables for formatting, Stack or heap
*/
#define __sprintf(dest, format, ...) \
		_sprintf(dest, format, (void *[]){__VA_ARGS__, 0});
```

### Functions
```c
fn 		ptr_to_str(ptr p, string out);
string	int_to_str(int num);
fn 		_sprintf(string buffer, string format, any* args);
fn 		istr(string dest, int num);
len_t 	str_len(string buffer);
string 	str_dup(const string buffer);
int   	str_append(string src, const string sub);
bool	str_cmp(const string src, const string needle);
pos_t 	find_string(const string buff, const string needle);
sArr 	split_lines(const string buffer, int* idx);
sArr 	split_string(const string buffer, const char ch, int* idx);
string 	get_sub_str(const string buffer, int start, int end);
bool 	is_empty(string buffer);
```