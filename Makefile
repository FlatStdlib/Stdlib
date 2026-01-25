###
#
#       Libbase
#
###
# Compiler
COMPILER 	= gcc

# Build Path
BUILD 		= build

# Library Filename & External Library Path
# Header file default path
LIB			= libbase.a
LIB_PATH	= /usr/lib
HEADER_PATH = /usr/local/include

# Object Filename and Path
OBJ			= libbase.o
OBJ_PATH	= $(BUILD)

#
# Base-GCC Executable (DO NOT CHANGE)
# Compile and Link sources with libbase
#
GBASE_EXEC 	= lbg
GBASE_OBJ	= lbg.o
GBASE_PATH 	= /bin/lbg	# Final Executable Path

# Compilation flags and files
FLAGS 		= -c -nostdlib -nostdinc
FILES 		= src/c/*.c \
			  src/c/stdlib/*.c \

.PHONY: all

# Default Installer
all: setup compile cloader move clean compile_asm # test_run

edit:
	nano -m *.md Makefile *.py **/*.h **/*.c src/asm/*.asm src/c/*.c src/c/stdlib/*.c

setup:
	mkdir -p $(BUILD)

# Count Source Code Lines
count:
	wc -l \
	tests/*.c \
	src/c/*.c \
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
	gcc -c ../lbg-compiler/loader.c -o $(BUILD)/loader.o -nostdlib -ffunction-sections -Wl,--gc-sections
	gcc -c ../lbg-compiler/lbg.c -o $(GBASE_OBJ) -nostdlib -ffunction-sections -Wl,--gc-sections -fdata-sections
# 	cp $(BUILD)/clibp.o cpy.o
	ld --gc-sections -o $(GBASE_EXEC) $(GBASE_OBJ) $(BUILD)/$(LIB) $(BUILD)/loader.o

#"-ffunction-sections", "-fdata-sections", "-Wl,--gc-sections",

#
# Set header files in /usr/local/include/
# Set library binary in /usr/lib
# Set compiler-linker in /bin/
# Set executable perms
#
move:
	cp -r headers/* $(HEADER_PATH)
	cp $(BUILD)/$(LIB) $(LIB_PATH)
	cp $(BUILD)/loader.o $(LIB_PATH)
	cp $(GBASE_EXEC) $(GBASE_PATH) 
	chmod +x $(GBASE_PATH)

#
# Delete obj file
#
clean:
	rm -rf *.o
	rm $(GBASE_EXEC)

#
# Test all test files in 'tests/'
#
test_run:
	gclibp tests/heap.c -o heap && ./heap
	gclibp tests/file.c -o file && ./file
	gclibp tests/thread.c -o thread && ./thread
	rm -rf heap file thread
