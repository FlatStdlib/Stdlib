.PHONY: all

all: compile_asm_libs compile

clean:
	rm -rf build

dir:
	mkdir -p build

move:
	cp -r headers/* /usr/local/include

test:
	gcc -nostdlib t.c -o build/app \
	src/*.c src/**/*.c \
	build/lib.o

count:
	wc -l t.c \
	src/*.c \
	src/stdlib/*.c \
	headers/*.h \
	headers/stdlib/*.h

#
# Compile x86 and x86_64 ASM libs to object file
#
compile_asm_libs:
	nasm -f elf64 asm/x86.asm -o build/x86.o
	nasm -f elf64 asm/x86_64.asm -o build/x86_64.o

#
# Compile the current arch core ASM lib for clib+ built-in
# Merge clibp+ built-in lib for the compiler and for the system to use clib+ with other compilers
#

compile:
	nasm -f elf64 asm/x86_64/lib.asm -o build/lib.o

	rm -rf *.o
	gcc -c src/*.c \
	src/stdlib/*.c \
	src/libs/*.c \
	-nostdlib
	ar rcs build/libclibp.a *.o
	ar rcs build/clibp.o *.o
	rm -rf *.o

#	gcc t.c -o t \
#	src/*.c \
#	src/stdlib/*.c \
#	src/libs/*.c \
#	build/lib.o \
#	-ggdb

#
# Compile clib+ compiler-linker into a object file
# Compile the loader into a object file
# Compile the clib+ compiler-linker with the built-in lib obj file
# clean-up
#
cloader:
	gcc -c linker/gcc_clibp.c -o gcc_clibp.o -nostdlib
	gcc -c loader/loader.c -o loader/loader.o -nostdlib -ffunction-sections -Wl,--gc-sections
	ld -o gcc_clibp gcc_clibp.o build/lib.o
	rm gcc_clibp.o
