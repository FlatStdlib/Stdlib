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
* [Memory](/doc/stdlib_docs/memory.md)
* [Int](/doc/stdlib_docs/int.md)
* [String](/doc/stdlib_docs/string.md)
* [Array](/doc/stdlib_docs/array.md)
* [Map](/doc/stdlib_docs/map.md)
* [File](/doc/stdlib_docs/file.md)
* [Thread](/doc/stdlib_docs/thread.md)
* [Socket](#/doc/stdlib_docs/socket.md)
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

# Definitions
```c
#define AF_INET         2
#define SOL_SOCKET      1
#define SO_REUSEADDR    2
#define SO_RCVTIMEO 	20
```

# Type(s) / Struct
```c
typedef struct {
    u16  		sun_family;
    char 		sun_path[108];
} _sockaddr_un;

typedef struct {
    u16 		sin_family;
    u16 		sin_port;
    struct {
		u32 	s_addr;
	} 			sin_addr;
    u8 			sin_zero[8];
} _sockaddr_in;

typedef _sockaddr_in addr_in;

typedef struct {
    u16 		sin6_family;
    u16 		sin6_port;
    i32 		sin6_flowinfo;
    struct {
		u8 		s6_addr[16];
	} 			sin6_addr;
    i32 		sin6_scope_id;
} _sockaddr_in6;

typedef struct {
	int         fd;
	addr_in		addr;

	int			buff_len;
} _sock_t;

typedef _sock_t sock;
typedef _sock_t *sock_t;
```

# Functions
```c
sock_t 		listen_tcp(const string ip, int port, int concurrent);
sock_t 		sock_accept(sock_t sock, len_t len);
int 		sock_write(sock_t sock, string buffer);
string 		sock_read(sock_t sock);
int 		parse_ipv4(const char* ip, u32 *out);
char* 		convert_ip(u32 ip);
u16			_htons(u16 x);
u32 		_htonl(u32 x);
fn 			sock_close(sock_t);
```