###
#
# 		clib+ Installer
#
###
.PHONY: all

# Default Installer
all: dir compile cloader move clean

# TCC (In Testing Stage) 'sudo make'
tcc: tcc_compile

#
#
#	TCC Compilation
#
# 	- Still in development
#	- Unable to compile @ gcc_clibp with the backend
#
tcc_compile:
	mkdir -p build

	tcc -ffreestanding -std=c99 -c src/*.c \
	src/stdlib/*.c \
	src/libs/*.c \
	-nostdlib
# 	execstack -c *.o

	ar rcs build/libclibp.a *.o
	ar rcs build/clibp.o *.o
	rm -rf *.o

	tcc -ffreestanding -std=c99 -c linker/gcc_clibp.c -o gcc_clibp.o -ffunction-sections -Wl
	tcc -ffreestanding -std=c99 -c loader/loader.c -o loader/loader.o -ffunction-sections -Wl
	execstack -c gcc_clibp.o
	execstack -c loader/loader.o

	ld -o gcc_clibp gcc_clibp.o build/clibp.o
	rm gcc_clibp.o

	cp -r headers/* /usr/local/include
	cp build/libclibp.a /usr/lib

	rm -rf *.o

#
# Delete obj file
#
clean:
	rm -rf *.o

#
# Delete build dir
#
reset:
	rm -rf build
	rm -rf loader/*.o

#
# Create build dir
#
dir:
	mkdir -p build

#
# Set header files for clib+
# Set clib+ obj file to system-wide lib dir
#
move:
	cp -r headers/* /usr/local/include
	cp build/libclibp.a /usr/lib
	cp gcc_clibp /bin
	chmod +x /bin/gcc_clibp

count:
	wc -l \
	tests/*.c \
	src/*.c \
	src/stdlib/*.c \
	headers/*.h \

#
# Compile the current arch core ASM lib for clib+ built-in
# Merge clib+ built-in lib for the compiler and another for external use with other compilers
#

compile:
	gcc -c src/*.c \
	src/stdlib/*.c \
	src/libs/*.c \
	-nostdlib -nostdinc
	rm -rf build/libclibp.a
	rm -rf build/clibp.o
	ar rcs build/libclibp.a *.o
	ar rcs build/clibp.o *.o
	rm -rf *.o

#
# Compile clib+ compiler-linker into a object file
# Compile the loader into a object file
# Compile the clib+ compiler-linker with the built-in lib obj file
# clean-up
#
cloader:
	gcc -c linker/gcc_clibp.c -o gcc_clibp.o -nostdlib
	gcc -c loader/loader.c -o loader/loader.o -nostdlib -ffunction-sections -Wl,--gc-sections
	gcc -c loader/init_loader.c -o loader/init_loader.o -nostdlib -ffunction-sections -Wl,--gc-sections -nostdlib
	ld -o gcc_clibp gcc_clibp.o build/clibp.o
	rm gcc_clibp.o

test_run:
	sudo make && ./gcc_clibp tests/heap.c -o t && ./t
	c99 tests/heap.c -o t -lclibp && ./t

riscv-gcc:
	mkdir -p build

	clang --target=riscv64-unknown-elf \
	-c src/*.c \
	src/stdlib/*.c \
	src/libs/*.c \
	-nostdlib -nostdinc

	ar rcs build/riscv_libclibp.a *.o
	ar rcs build/riscv_clibp.o *.o
	rm -rf *.o

	clang --target=riscv64-unknown-elf -c linker/gcc_clibp.c -o riscv_gcc_clibp.o -nostdlib

	riscv64-unknown-elf-ld -o riscv_gcc_clibp riscv_gcc_clibp.o build/*.o

	rm -rf *.o
