* A Documentation for developers who know what they're doing!

* This documentation MD was generated using ``_test.py``

# _CLIBP_INTERNAL_H
```c
	#define clibp_panic(msg) 	\
				__clibp_panic(msg, __FILE__, __LINE__);

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
# _CLIBP_MEM_H
```c
	fn 		memzero(any ptr, size_t);
	int 	mem_cmp(any src, any ptr, size_t size);
	fn 		mem_cpy(any dest, any src, size_t size);
	fn 		mem_set(any ptr, char ch, size_t size);

	int 	get_input(string dest, len_t count);
```
# _CLIBP_ALLOCATOR_H
```c
	#if defined(_C_MALLOC_ALTERNATIVE)
		#define malloc allocate
	#endif

	#define PROT_READ   	0x1
	#define PROT_WRITE		0x2
	#define PROT_EXEC   	0x4
	#define PROT_NONE   	0x0

	#define MAP_SHARED  	0x01
	#define MAP_PRIVATE 	0x02
	#define MAP_FIXED   	0x10
	#define MAP_ANONYMOUS 	0x20
	#define MAP_STACK 		0x20000

	typedef void* heap_t;
	extern heap_t               _HEAP_;

	#define _STANDARD_MEM_SZ_   4096
	#define _LARGE_MEM_SZ_      4096 * 3
	extern int					_HEAP_PAGE_;
	extern int                  _HEAP_SZ_;
	extern int                  _HEAP_PAGE_SZ_;

	typedef struct {
		int     size;
		size_t  length;
		int     id;
	} __meta__;

	extern const int            HEAP_META_SZ;
	extern int                  used_mem;

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
# _CLIBP_INT_H
```c
i32		count_int_digits(i32 num);
```
# _CLIBP_CHAR_H
```c
	i32 	is_ascii(const char c);
	i32 	is_ascii_alpha(const char c);
	i32 	count_char(const string buffer, const char ch);
	i32 	find_char(const string buffer, const char ch);
	i32 	find_char_at(const string buffer, const char ch, int match);
	i32 	_find_char_at(const string buffer, const char ch, int match, int *start);
	int 	replace_char(string buffer, const char find, const char replace);
```
# _CLIBP_STR_H
```c
	#define __sprintf(dest, format, ...) \
			_sprintf(dest, format, (void *[]){__VA_ARGS__, 0});

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
# _CLIBP_ARR_H
```c
	array 	init_array(void);
	array	array_append(array arr, ptr p);
	int 	array_contains_ptr(array arr, ptr p);
	int 	array_contains_str(array arr, string needle);
```
# _CLIBP_MAP_H
```c
	typedef struct {
		string key;
		string value;
	} _field;

	typedef _field field;
	typedef _field *field_t;
	typedef _field **fields_t;

	typedef struct {
		fields_t 	fields;
		int 		len;
	} _map;

	typedef _map map;
	typedef _map *map_t;

	map_t 	init_map(void);
	bool 	map_append(map_t map, string key, string value);
	string 	find_key(map_t map, string key);
```
# _CLIBP_FILE_H
```c
	typedef u32 fd_t;

	typedef enum FILE_MODE {
		O_RDONLY 	= 0,		// Read
		O_WRONLY 	= 01,		// Write
		O_RDWR 		= 02,		// Read-Write
		O_CREAT 	= 0100,		// Create
		O_EXCL 		= 0200,
		O_NOCTTY 	= 0400,
		O_TRUNC 	= 01000		// Truncate
	} FILE_MODE;

	fd_t	open_file(const char* filename, FILE_MODE mode, int flags);

	int		file_content_size(fd_t fd);

	int		file_read(fd_t fd, char* buffer, int sz);

	#define file_uc_read(fd, buff, sz) file_read

	int		file_write(fd_t fd, const char* buffer, len_t len);

	fn		file_close(fd_t fd);
```
# _CLIBP_SOCKET_H
```c
	
	#define AF_INET         2
	#define SOL_SOCKET      1
	#define SO_REUSEADDR    2
	#define SO_RCVTIMEO 	20

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
# _CLIBP_THREAD_H
```c
	typedef struct
	{
		handler_t	fnc;
		ptr			arguments;
		i8 			wait;
		i8 			finished;
		i32			pid;
		i32			ttid;
	} _thread;
	
	typedef _thread 	thread;
	typedef thread 		*thread_t;
	typedef thread 		**threads_t;

	typedef struct
	{
		threads_t	threads;
		int			idx;
	} gthread;

	gthread 	init_group_thread();
	bool 		append_thread(gthread *g, thread_t t);

	thread 		start_thread(handler_t fnc, ptr p, int wait);
	fn 			thread_kill(thread_t t);
```
