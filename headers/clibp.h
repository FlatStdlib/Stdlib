/*
*
*	[ clib+ ]
*
*	- An alternative minimal C backend ( -nostdlib )
*
*/
#pragma once

extern int __CLIBP_DEBUG__;
#ifndef __CLIBP__
	#define __CLIBP__
	#define _CLIBP_CHAR_H
	#define _CLIBP_STR_H
	#define _CLIBP_ARR_H
	#define _CLIBP_MEM_H
	#define _CLIBP_FILE_H
	#define _CLIBP_SOCKET_H
	#define _CLIBP_INTERNAL_H
	#define _CLIBP_ALLOCATOR_H

	#define printf print
	#define nullptr_t ((void *)0)
	#define emptyptr_t ((void *)-1)
#endif

/*
    Auto Architecture Detection

    Disable by using DISABLE_AUTO_ARCH_DET
    followed by a specific architecture for compilation
*/
#include "asm.h"

/*
	Built-in Types
*/

#define bool 				int
#define true 				1
#define false 				0

typedef signed char			i8;
typedef signed short int	i16;
typedef signed int 			i32;
typedef signed long long 	i64;

typedef unsigned char 		u8;
typedef unsigned short 		u16;
typedef unsigned int		u32;
typedef unsigned long long 	u64;

/* string */
typedef char				*string;

/* general array */
typedef i32					*array;

/* int array and char array */
typedef i32					*iArr;
typedef string				*sArr;

typedef void 				*any;
typedef void 				fn;
typedef void 				*(*handler_t)();

/* backend purposes only */
typedef void 				*ptr;

/* Counters */
typedef i32 				len_t;
typedef i32					pos_t;

/*
	Compiler Detection
    Implment C Types when using -nostdlib -nostdinc
*/
#if defined(__TINYC__) || defined(__GNUC__)
	/* Alot of libc libs, have __GLIBC_INTERNAL_STARTING_HEADER_IMPLEMENTATION */
    #if !defined(_STDIO_H) || !defined(__GLIBC_INTERNAL_STARTING_HEADER_IMPLEMENTATION)
    	#undef __GLIBC_INTERNAL_STARTING_HEADER_IMPLEMENTATION
        #define NULL                    ((void *)0)
    #endif

	/* Implementation of the following types from stdint.h */
//    #if !defined(_STDINT_H) && !defined(__SIZE_TYPE__)
		typedef unsigned long int		size_t;
		typedef unsigned long int		uintptr_t;
//    #endif
#endif

/* Global Function Declaraction */
long _syscall(long n, long a1, long a2, long a3, long a4, long a5, long a6);
fn __syscall(long syscall, long arg1, long arg2, long arg3, long arg4, long arg5, long arg6);
long __syscall__(long arg1, long arg2, long arg3, long arg4, long arg5, long arg6, long sys);

// Get Start-up App Cmdline Arguments
int 	get_args(char *argv[]);

#ifdef _CLIBP_INTERNAL_H
	#define clibp_panic(msg) 	\
			__clibp_panic(msg, __FILE__, __LINE__);

	/* internal.c */
	fn		toggle_debug_mode();
	fn 		ptr_to_str(void *p, char *out);
	fn 		__exit(int code);
	fn 		print_sz(const string buffer, int sz);
	fn		printc(const char ch);
	fn 		printi(int num);
	fn 		_printi(int value);
	string	int_to_str(int num);
	fn 		print(const string buff);
	fn		println(const string buff);
	fn 		printsz(const string buff, int sz);
	fn 		print_args(sArr arr);
	ptr		to_heap(ptr p, i32 sz);
	fn		__clibp_panic(string msg, string file, int line);
#endif

/*
		Memory Utilities
	@File: src/mem.c
*/
#ifdef _CLIBP_MEM_H
	/* General memory functions */
	fn 		memzero(any ptr, size_t);
	int 	mem_cmp(any src, any ptr, size_t size);
	fn 		mem_cpy(any dest, any src, size_t size);
	fn 		mem_set(any ptr, char ch, size_t size);

	int 	get_input(string dest, len_t count);
#endif

/*
		Allocator
	@File: src/allocator.c
*/
#ifdef _CLIBP_ALLOCATOR_H
	#if defined(_C_MALLOC_ALTERNATIVE)
	    #define malloc allocate
	#endif
	typedef void                *heap_t;
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
	extern int                  HEAP_DEBUG;
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
	__meta__    *__get_meta__(any ptr);
#endif

#ifdef _CLIBP_CHAR_H
	i32 	is_ascii(const char c);
	i32 	is_ascii_alpha(const char c);
	i32 	count_char(const string buffer, const char ch);
	i32 	find_char(const string buffer, const char ch);
	i32 	find_char_at(const string buffer, const char ch, int match);
#endif

/* stdlib/str.c */
#ifdef _CLIBP_STR_H
	#define __sprintf(dest, format, ...) \
		_sprintf(dest, format, (void *[]){__VA_ARGS__, 0});

	fn 		_sprintf(string buffer, string format, any *args);
	fn 		istr(char *dest, int num);
	len_t 	str_len(string buffer);
	string 	str_dup(const string buffer);
	int   	str_append(string src, const string sub);
	bool	str_cmp(const string src, const string needle);
	pos_t 	find_str(const string buff, const string needle);
	sArr 	split_lines(const string buffer, int *idx);
	sArr 	split_string(const string buffer, const char ch, int *idx);
#endif

#ifdef _CLIBP_ARR_H
	int 	arr_contains(sArr args, string needle);
#endif

#ifdef _CLIBP_FILE_H
	typedef unsigned int fd_t;

	typedef enum FILE_MODE {
		O_RDONLY		= 0,		// Read
		O_WRONLY	    = 01,		// Write
		O_RDWR		    = 02,		// Read-Write
		O_CREAT		   	= 0100,		// Create
		O_EXCL		   	= 0200,
		O_NOCTTY	   	= 0400,
		O_TRUNC		  	= 01000		// Truncate
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
	fd_t	open_file(const char *filename, FILE_MODE mode, int flags);

	/*
		[@DOC]
			int file_content_size(fd_t fd);

			Desc;
				get file size

			return;
				>0 on success
				-1 on fail
	*/
	int		file_content_size(fd_t fd);

	/*
		[@DOC]
			int file_read(fd_t fd, char *buffer, int sz);

			Desc;
				get file content

			return;
				>0 on scuess
				<=0 on fail
	*/
	int		file_read(fd_t fd, char *buffer, int sz);

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
	int		file_write(fd_t fd, const char *buffer, len_t len);

	/*
		[@DOC]
			int file_close(fd_t fd, const char *buffer, len_t len);

			Desc;
				close file
	*/
	fn		file_close(fd_t fd);
#endif

#ifdef _CLIBP_SOCKET_H
	typedef struct {
	    u16                 sun_family;
	    char                sun_path[108];
	} sockaddr_un;

	typedef struct {
	    unsigned short int  sin_family;
	    unsigned short int  sin_port;
	    unsigned int        sin_addr;
	    unsigned char       sin_zero[8];
	} sockaddr_in;

	typedef struct {
	    unsigned short int  sin6_family;
	    unsigned short int  sin6_port;
	    unsigned int        sin6_flowinfo;
	    unsigned char       sin6_addr[16];
	    unsigned int        sin6_scope_id;
	} sockaddr_in6;

	typedef struct {
	    int             fd;
	    sockaddr_in     addr;

	    int             buff_len;
	} _sock_t;

	typedef _sock_t *sock_t;
	_sock_t listen_tcp(const string ip, int port, int concurrent);
	_sock_t sock_accept(_sock_t sock, len_t len);
	int sock_write(_sock_t sock, string buffer);
	string sock_read(_sock_t sock);
    int parse_ipv4(const char *ip, unsigned int *out);
    char *convert_ip(unsigned int ip);
    unsigned short _htons(unsigned short x);
    unsigned int _htonl(unsigned int x);
    fn sock_close(_sock_t);
#endif
