/*
*
*	[ clib+ ]
*
*	- An alternative minimal C backend ( -nostdlib -nostdinc )
*
*/
#pragma once

extern char _OUTPUT_[1024];
extern int __LB_DEBUG__;
#ifndef __LB__
	#define __LB__
	#define _LB_INT_H
	#define _LB_CHAR_H
	#define _LB_STR_H
	#define _LB_ARR_H
	#define _LB_MAP_H
	#define _LB_MEM_H
	#define _LB_FILE_H
	#define _LB_SOCKET_H
	#define _LB_THREAD_H
	#define _LB_INTERNAL_H
	#define _LB_ALLOCATOR_H

	
	#define _printf(format, ...) \
			_sprintf(_OUTPUT_, format, (void *[]){__VA_ARGS__, 0}); \
			print(_OUTPUT_);
	#define nullptr_t ((void *)0)
	#define emptyptr_t ((void *)-1)
#endif

/*
	Auto Architecture Detection

	Disable by using DISABLE_AUTO_ARCH_DET
	followed by a specific architecture for compilation
*/
#ifndef __NO_ASM__
	#define __NO_ASM__
	#include "asm.h"
#endif

/*
	Built-in Types
*/
typedef signed char			i8;
typedef signed short int	i16;

#define i32_MIN 			-0x80000000
#define i32_MAX 			0x7FFFFFFF
typedef signed int 			i32;

typedef signed long long 	i64;

typedef unsigned char 		u8;
typedef unsigned short 		u16;
typedef unsigned int		u32;
typedef unsigned long long 	u64;

/* string */
typedef char* 				string;

/* general array */
typedef void** 				array;

/* int array and char array */
typedef i32* 				iArr;
typedef string* 			sArr;

typedef void* 				any;

#define public
#define private static
typedef void 				fn;
typedef void* 				(*handler_t)();

/* Backend and allocator Purposes Only */
typedef void* 				ptr;

/* Counters */
typedef i32 				len_t;
typedef i32					pos_t;

#define bool				i8
#define true				1
#define false				0

/*
	Compiler Detection - Disable GLIBC Shit
*/
#if defined(__TINYC__) || defined(__GNUC__)
	/* 
		Alot of libc libs, have __GLIBC_INTERNAL_STARTING_HEADER_IMPLEMENTATION 
		Disable it
	*/
	#undef __GLIBC_INTERNAL_STARTING_HEADER_IMPLEMENTATION
	#define NULL                    ((void *)0)

	// Redeclaring from stddef.h exactly the same to avoid name confliction
	typedef unsigned long int		size_t;
	typedef unsigned long int		uintptr_t;
#endif

/* Global Function Declaraction */
long _syscall(long n, long a1, long a2, long a3, long a4, long a5, long a6);
fn __syscall(long syscall, long arg1, long arg2, long arg3, long arg4, long arg5, long arg6);
long __syscall__(long arg1, long arg2, long arg3, long arg4, long arg5, long arg6, long sys);
long ___syscall__(long arg1, long arg2, long arg3, long arg4, long arg5, long arg6, long sys);

// Get Start-up App Cmdline Arguments
int 	get_args(char* argv[]);

#ifdef _LB_INTERNAL_H
	#define lb_panic(msg) 	\
		__lb_panic(msg, __FILE__, __LINE__);

	/* internal.c */
	public fn		toggle_debug_mode();
	public fn 		__exit(int code);
	public fn 		execute(string app, sArr args);
	public fn 		print_sz(const string buffer, int sz);
	public fn		printc(const char ch);
	public fn 		printi(int num);
	public fn 		_printi(int value);
	public fn 		print(const string buff);
	public fn		println(const string buff);
	public fn 		printsz(const string buff, int sz);
	public fn 		print_args(sArr arr);
	public ptr		to_heap(ptr p, i32 sz);
	public fn		__lb_panic(string msg, string file, int line);
#endif

/*
		Memory Utilities
	@File: src/mem.c
*/
#ifdef _LB_MEM_H
	/* General memory functions */
	public fn 		memzero(any ptr, size_t);
	public int 		mem_cmp(any src, any ptr, size_t size);
	public fn 		mem_cpy(any dest, any src, size_t size);
	public fn 		mem_set(any ptr, char ch, size_t size);

	public int 		get_input(string dest, len_t count);
#endif

/*
		Allocator
	@File: src/allocator.c
*/
#ifdef _LB_ALLOCATOR_H
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

	public fn 			set_heap_sz(int n);
	public fn 			set_heap_debug(void);
	private fn 			req_memory(void);

	public fn        	init_mem(void);
	public fn        	uninit_mem(void);
	public int         	__get_total_mem_used__(void);
	private int			find_space(int space);
	public ptr         	allocate(int sz, int len);
	public ptr  		reallocate(any p, int sz);
	public int         	__get_size__(any ptr);
	public int         	__is_heap_init__();
	public fn 			pfree_array(array p);
	public fn 			_pfree(any ptr);
	public fn        	pfree(any ptr, int clean);
	public __meta__* 	__get_meta__(any ptr);
#endif

/*
			int
	[ src/stdlib/int.c ]
*/
#ifdef _LB_INT_H
	public i32		count_int_digits(i32 num);
	public int		str_to_int(const char *s);
#endif

/*
	 		char
	[ src/stdlib/char.c ]
*/
#ifdef _LB_CHAR_H
	public i32 		is_ascii(const char c);
	public i32 		is_ascii_alpha(const char c);
	public i32 		count_char(const string buffer, const char ch);
	public i32 		find_char(const string buffer, const char ch);
	public i32 		find_char_at(const string buffer, const char ch, int match);
	public i32 		_find_char_at(const string buffer, const char ch, int match, int *start);
	public int 		replace_char(string buffer, const char find, const char replace);
#endif

/*
	 		string
	[ src/stdlib/string.c ]
*/
#ifdef _LB_STR_H
	#define __sprintf(dest, format, ...) \
			_sprintf(dest, format, (void *[]){__VA_ARGS__, 0});

	public fn 		ptr_to_str(ptr p, string out);
	public string	int_to_str(int num);
	public fn 		_sprintf(string buffer, string format, any* args);
	public fn 		str_append_int(string dest, int num);
	public len_t 	str_len(string buffer);
	public string 	str_dup(const string buffer);
	public bool 	str_append_array(string buff, const array arr);
	public bool   	str_append(string src, const string sub);
	public bool		str_cmp(const string src, const string needle);
	public pos_t 	find_string(const string buff, const string needle);
	public sArr 	split_lines(const string buffer, int* idx);
	public sArr 	split_string(const string buffer, const char ch, int* idx);
	public string 	get_sub_str(const string buffer, int start, int end);
	public bool 	is_empty(string buffer);
	public bool 	str_startswith(string buffer, string needle);
	public bool 	str_endswith(string buffer, string needle);
	public fn 		byte_to_hex(u8 byte, string out);
#endif

#ifdef _LB_ARR_H
	public array 	init_array(void);
	public array	array_append(array arr, ptr p);
	public int 		array_contains_ptr(array arr, ptr p);
	public int 		array_contains_str(array arr, string needle);
#endif

#ifdef _LB_MAP_H
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

	public map_t 	init_map(void);
	public bool 	map_append(map_t map, string key, string value);
	public string 	find_key(map_t map, string key);
	public fn 		field_destruct(field_t field);
	public fn 		map_destruct(map_t map);
#endif

#ifdef _LB_FILE_H
	typedef u32 fd_t;

	typedef enum FILE_MODE {
		_O_RDONLY 	= 0,		// Read
		_O_WRONLY 	= 01,		// Write
		_O_RDWR 		= 02,	// Read-Write
		_O_CREAT 	= 0100,		// Create
		_O_EXCL 		= 0200,
		_O_NOCTTY 	= 0400,
		_O_TRUNC 	= 01000		// Truncate
	} FILE_MODE;

	/*
		[@DOC]
			fd_t open_file(const char *filename, FILE_MODE mode);

			Desc;
				open a file stream

			return;
				>0 on sucess
				-1 on fail
	*/
	public fd_t		open_file(const char* filename, FILE_MODE mode, int flags);

	/*
		[@DOC]
			int file_content_size(fd_t fd);

			Desc;
				get file size

			return;
				>0 on success
				-1 on fail
	*/
	public int		file_content_size(fd_t fd);

	/*
		[@DOC]
			int file_read(fd_t fd, char *buffer, int sz);

			Desc;
				get file content

			return;
				>0 on scuess
				<=0 on fail
	*/
	public int		file_read(fd_t fd, char* buffer, int sz);

	/* file_read for unsigned char */
	#define file_uc_read(fd, buff, sz) file_read

	/*
		[@DOC]
			int file_write(fd_t fd, const char *buffer, len_t len);

			Desc;
				write to file

			return;
				>0 on success
				-1 on fail
	*/
	public int		file_write(fd_t fd, const char* buffer, len_t len);

	/*
		[@DOC]
			int file_close(fd_t fd, const char *buffer, len_t len);

			Desc;
				close file
	*/
	public fn		file_close(fd_t fd);
#endif

#ifdef _LB_SOCKET_H
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

	public sock_t 		listen_tcp(const string ip, int port, int concurrent);
	public sock_t 		sock_accept(sock_t sock, len_t len);
	public int 			sock_write(sock_t sock, string buffer);
	public string 		sock_read(sock_t sock);
	public int 			parse_ipv4(const char* ip, u32 *out);
	public string 		convert_ip(u32 ip);
	public u16			_htons(u16 x);
	public u32 			_htonl(u32 x);
	public fn 			sock_close(sock_t);
#endif

#ifdef _LB_THREAD_H
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

	public gthread 		init_group_thread();
	public bool 		append_thread(gthread *g, thread_t t);

	public thread 		start_thread(handler_t fnc, ptr p, int wait);
	public fn 			thread_kill(thread_t t);
#endif
