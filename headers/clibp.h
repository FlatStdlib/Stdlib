/*
*
*	[ clib+ ]
*
*	- An alternative minimal C backend ( -nostdlib )
*
*/
#pragma once

#ifndef __CLIBP__
	#define __CLIBP__
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
	Implment C Types when using -nostdlib -nostdinc
*/
#if defined(__TINYC__) || defined(__GNUC__)
	#if !defined(__GLIBC_INTERNAL_STARTING_HEADER_IMPLEMENTATION)
		#define NULL 					((void *)0)

		/* From stdint.h */
		typedef unsigned long int       uintptr_t;
	#endif
#endif

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
typedef u8					*string;

/*
	Temporary
	TODO; change str to string type
	TODO; change fn_t to fn
*/
typedef char 				*str;
typedef void 				fn_t;

/* general array */
typedef u32					*array;

/* int array and char array */
typedef i32					*iArr;
typedef u8					**sArr;

typedef void 				none;
typedef void 				*any;
typedef void 				fn;
typedef void 				*handler_t;
typedef void 				*ptr;

/* Counters */
typedef i32 				size_t;
typedef i32 				len_t;
typedef i32					pos_t;

/*
    Temporary
    TODO; change str to string type
    TODO; change fn_t to fn
*/
typedef char                *str;
typedef void                fn_t;

/* Global Function Declaraction */
long _syscall(long n, long a1, long a2, long a3, long a4, long a5, long a6);
void __syscall(long syscall, long arg1, long arg2, long arg3, long arg4, long arg5, long arg6);
long __syscall__(long arg1, long arg2, long arg3, long arg4, long arg5, long arg6, long sys);

// Get Start-up App Cmdline Arguments
int 	get_args(char *argv[]);

#ifdef _CLIBP_INTERNAL_H
	/* internal.c */
	void 	ptr_to_str(void *p, char *out);
	none 	__exit(int code);
	none 	print_sz(const str buffer, int sz);
	none 	printc(const char ch);
	none 	printi(int num);
	none 	_printi(int value);
	none 	print(const str buff);
	none	println(const str buff);
	none 	printsz(const str buff, int sz);
	none 	printa(const str *buff);
	none 	err_n_exit(const str buff, int code);
#endif

/*
		Memory Utilities
	@File: src/mem.c
*/
#ifdef _CLIBP_MEM_H
	/* General memory functions */
	none 	memzero(any ptr, size_t);
	int 	mem_cmp(any src, any ptr, size_t size);
	none 	mem_cpy(any dest, any src, size_t size);
	none 	mem_set(any ptr, char ch, size_t size);

	int 	get_input(str dest, len_t count);
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
	extern int                  _HEAP_SZ_;
	extern int                  _HEAP_PAGE_SZ_;

	typedef struct {
	    int     size;
	    size_t  length;
	    int     id;
	} __meta__;

	extern int                  HEAP_META_SZ;

	typedef void                *heap_t;

	extern heap_t               _HEAP_;
	extern int                  HEAP_DEBUG;
	extern int                  used_mem;

	void set_heap_sz(int n);

	void        init_mem();
	void        uninit_mem();
	int         __get_total_mem_used__(void);
	ptr         allocate(int sz, int len);
	int         __get_size__(any ptr);
	int         __is_heap_init__();
	void        pfree(any ptr);
	__meta__    *__get_meta__(any ptr);
#endif

/* stdlib/str.c */
#ifdef _CLIBP_STR_H
	#define __sprintf(dest, format, ...) \
		_sprintf(dest, format, (void *[]){__VA_ARGS__, 0});

	fn_t 	_sprintf(str buffer, str format, any *args);
	fn_t 	istr(char *dest, int num);
	len_t 	str_len(str buffer);
	int   	stra(str src, const str sub);
	bool	str_cmp(const str src, const str needle);
	pos_t 	find_char(const str buff, const char ch, int match);
	pos_t 	find_str(const str buff, const str needle);
#endif

#ifdef _CLIBP_ARR_H
	typedef any *arr;
	int 	arr_contains(arr args, str needle);
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
	fd_t	open_file(const char *filename, FILE_MODE mode);

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
	void	file_close(fd_t fd);
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
	_sock_t listen_tcp(const str ip, int port, int concurrent);
    int parse_ipv4(const char *ip, unsigned int *out);
    char *convert_ip(unsigned int ip);
    unsigned short _htons(unsigned short x);
    unsigned int _htonl(unsigned int x);
#endif
