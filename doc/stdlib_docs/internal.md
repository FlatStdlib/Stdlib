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

# Internal Library

### Definitions
```c
#define clibp_panic(msg) 	\
			__clibp_panic(msg, __FILE__, __LINE__);
```

### Functions
```c
/* internal.c */
fn		toggle_debug_mode();
fn 		__exit(int code);
fn 		execute(string app, sArr args);
fn 		print_sz(const string buffer, int sz);
fn		printc(const char ch);
fn 		printi(int num);
fn 		_printi(int value);
fn 		print(const string buff);
fn		println(const string buff);
fn 		printsz(const string buff, int sz);
fn 		print_args(sArr arr);
ptr		to_heap(ptr p, i32 sz);
fn		__clibp_panic(string msg, string file, int line);
```