.PHONY: all

all: dir compile move cloader move clean

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

count:
	wc -l t.c \
	src/*.c \
	src/stdlib/*.c \
	headers/*.h \
	headers/stdlib/*.h

#
# Compile the current arch core ASM lib for clib+ built-in
# Merge clib+ built-in lib for the compiler and another for external use with other compilers
#

compile:
	gcc -c src/*.c \
	src/stdlib/*.c \
	src/libs/*.c \
	-nostdlib
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
	gcc -c loader/loader.c -o loader/loader.o -nostdlib #-ffunction-sections -Wl,--gc-sections
	ld -o gcc_clibp gcc_clibp.o build/clibp.o
	rm gcc_clibp.o
