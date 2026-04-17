/*
	- The core of clib+ - targets host cpu info & architecture functionality
	- For Host Architecture, The plan is to have ASM Lib for all syscall to follow
	  ABI standard like the current x86 and x86_64 ASM lib in 'src/asm/'
		+ TODO; Split this up into single header files for each architecture
		  and use this for macros only
	- Each architecture has the following definition for syscall

	MAX_REGISTER 		| Used for the amount of arguments ABI takes for syscall
	SYSCALL_REGISTER 	| Register for syscall number
	EXECUTE_SYSCALL		| Syscall Execution

	_SYS_NAME			| A leading _SYS_ for the name of syscalls, all syscalls
						| must match from each architecture to avoid having to make
						| future edits

	__REGISTER__		| Leading and ending double-underscore for register name(s)
						| optional to add more cpu register beyond what ABI requires
						| for external uses
*/
#pragma once

struct sleep_t {
    long sec, nsec;
};

struct timeval {
    long tv_sec, tv_usec;
};

#if defined(__x86__) || defined(__i386__)
	#define __x86__
	#include "architectures/x86.h"
#elif defined(__x86_64__)
	#include "architectures/x86_64.h"
#elif defined(__riscv)
	#include "architectures/riscv.h"
#elif defined(__aarch64__)
	#include "architectures/aarch64.h"
#elif defined(__arm__)
	#include "architectures/arm.h"
#elif defined(_WIN64)
	#include "architectures/win64.h"
#else
	#error "Invalid architecture"
#endif

