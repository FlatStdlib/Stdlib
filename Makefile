###
#
#       clib+ Installer
#
###
# Compiler
COMPILER 	= gcc

# Build Path
BUILD 		= build

# Library Filename & External Library Path
# Header file default path
LIB			= libclibp.a
LIB_PATH	= /usr/lib
HEADER_PATH = /usr/local/include

# Object Filename and Path
OBJ			= clibp.o
OBJ_PATH	= $(BUILD)

# clib+ Executable (DO NOT CHANGE)
CLIBP_PATH  = /bin/gclibp

# Compilation flags and files
FLAGS 		= -c -nostdlib -nostdinc
FILES 		= src/c/*.c \
			  src/c/stdlib/*.c \
			  src/c/libs/*.c \

.PHONY: all

# Default Installer
all: setup compile cloader move clean compile_asm test_run

setup:
	mkdir -p $(BUILD)

# Count Source Code Lines
count:
	wc -l \
	tests/*.c \
	src/c/*.c \
	src/c/libs/*.c \
	src/c/stdlib/*.c \
	headers/*.h \
	src/asm/*.asm

#
# Compile ASM libs
#
compile_asm:
	nasm -f elf32 src/asm/x86.asm -o $(BUILD)/x86.o
	nasm -f elf64 src/asm/x86_64.asm -o $(BUILD)/x86_64.o

#
# Compile clib+
# Merge clib+ built-in lib for the compiler and another for external use with other compilers
#
compile:
	$(COMPILER) $(FLAGS) $(FILES)
	rm -rf $(BUILD)/$(LIB)
	rm -rf $(BUILD)/$(OBJ)
	ar rcs $(BUILD)/$(LIB) *.o
	ar rcs $(BUILD)/$(OBJ) *.o

#
# Compile linker and loader for clib+
# clean-up
#
cloader:
	gcc -c linker/gcc_clibp.c -o gclibp.o -nostdlib
	gcc -c linker/loader.c -o $(BUILD)/loader.o -nostdlib -ffunction-sections -Wl,--gc-sections
	cp $(BUILD)/clibp.o cpy.o
	ld -o gclibp gclibp.o cpy.o

#
# Set header files in /usr/local/include/
# Set library binary in /usr/lib
# Set compiler-linker in /bin/
# Set executable perms
#
move:
	cp -r headers/*.h $(HEADER_PATH)
	cp $(BUILD)/$(LIB) $(LIB_PATH)
	cp $(BUILD)/loader.o $(LIB_PATH)
	cp gclibp $(CLIBP_PATH)
	chmod +x $(CLIBP_PATH)

#
# Delete obj file
#
clean:
	rm -rf *.o
	rm gclibp

#
# Test all test files in 'tests/'
#
test_run:
	gclibp tests/heap.c -o heap && ./heap
	gclibp tests/file.c -o file && ./file
	gclibp tests/thread.c -o thread && ./thread
	rm -rf heap file thread
