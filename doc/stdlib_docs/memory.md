<div align="center">
    <h1>clib+ Libraries</h1>

[Start-up](/README.md)
| [Introduction](/doc/intro.md)
| [Quick Doc](/quick_doc.md)
| ● [Libraries](/doc/libraries.md)
| [Todo](/doc/todo.md)
</div>

## List Of Libraries Provided for clib+
<table align="center">
<tr><td valign=top>

<p>Stdlib<p>

* [Internal](/doc/stdlib_docs/internal.md)
* [Allocator](/doc/stdlib_docs/allocator.md)
* ► [Memory](/doc/stdlib_docs/memory.md)
* [Int](/doc/stdlib_docs/int.md)
* [String](/doc/stdlib_docs/string.md)
* [Array](/doc/stdlib_docs/array.md)
* [Map](/doc/stdlib_docs/map.md)
* [File](/doc/stdlib_docs/file.md)
* [Thread](/doc/stdlib_docs/thread.md)
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

# Memory Library

### Functions
```c
/* General memory functions */
fn 		memzero(any ptr, size_t);
int 	mem_cmp(any src, any ptr, size_t size);
fn 		mem_cpy(any dest, any src, size_t size);
fn 		mem_set(any ptr, char ch, size_t size);

int 	get_input(string dest, len_t count);
```