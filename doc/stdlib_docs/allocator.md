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
* ► [Allocator](/doc/stdlib_docs/allocator.md)
* [Memory](/doc/stdlib_docs/memory.md)
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

# Allocator Library

## Definitions
```c
#define PROT_READ   	0x1
#define PROT_WRITE		0x2
#define PROT_EXEC   	0x4
#define PROT_NONE   	0x0

#define MAP_SHARED  	0x01
#define MAP_PRIVATE 	0x02
#define MAP_FIXED   	0x10
#define MAP_ANONYMOUS 	0x20
#define MAP_STACK 		0x20000

#define _STANDARD_MEM_SZ_   4096
#define _LARGE_MEM_SZ_      4096 * 3
```

## Type(s) / Struct(s)
```c
typedef void* heap_t;

typedef struct {
	int     size;
	size_t  length;
	int     id;
} __meta__;
```

## Functions

```c
fn 			set_heap_sz(int n);
fn 			set_heap_debug();
fn 			req_memory();

fn        	init_mem();
fn        	uninit_mem();
int         __get_total_mem_used__(void);
ptr         allocate(int sz, int len);
int         __get_size__(any ptr);
int         __is_heap_init__();
fn        	pfree(any ptr, int clean);
__meta__* __get_meta__(any ptr);
```