# 32BIT : -m32 -fno-stack-protector -fomit-frame-pointer
###
#
#       Flat Stdlib
#
###

# Default Compiler
COMPILER = gcc

# Filename(s)
FGCC = fsl						# FSL-GCC COMPILER
FGCC_OBJ = fsl_gcc.o   			# FSL-GCC COMPILER
FGCC_SYSTEM_PATH = /bin/fsl		# FSL-GCC COMPILER
LIB = libfsl.a					# FSL
OBJ = fsl.o						# FSL

# Path(s)
BUILD = build
SYSTEM_LIB_DIR = /usr/lib
SYSTEM_HDR_DIR = /usr/include
OBJ_PATH = $(BUILD)

# Compilation flags and files
FLAGS = -c -nostdlib -nostdinc ${DEBUG} ${CFLAGS}
FGCC_FLAGS = -c -nostdlib -ffunction-sections -Wl,--gc-sections -fdata-sections ${DEBUG} ${CFLAGS}
FILES = src/c/*.c src/c/os/*.c src/c/stdlib/*.c

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
	gcc ${FGCC_FLAGS} ${DEBUG} ../fsl/loader.c -o $(BUILD)/loader.o
	gcc ${FGCC_FLAGS} ${DEBUG} ../fsl/fsl.c -o $(FGCC_OBJ)
# 	cp $(BUILD)/fsl.o cpy.o
	ld --gc-sections -o $(FGCC) $(FGCC_OBJ) $(BUILD)/$(LIB) $(BUILD)/loader.o

#"-ffunction-sections", "-fdata-sections", "-Wl,--gc-sections",

#
# Set header files in /usr/local/include/
# Set library binary in /usr/lib
# Set compiler-linker in /bin/
# Set executable perms
#
move:
	cp -r headers/* $(SYSTEM_HDR_DIR)
	cp $(BUILD)/$(LIB) $(SYSTEM_LIB_DIR)
	cp $(BUILD)/loader.o $(SYSTEM_LIB_DIR)
	cp $(FGCC) $(FGCC_SYSTEM_PATH) 
	chmod +x $(FGCC_SYSTEM_PATH)

#
# Delete obj file
#
clean:
	rm -rf *.o
	rm $(FGCC)

#
# Test all test files in 'tests/'
#
test_run:
	fsl tests/heap.c -o heap && ./heap
	fsl tests/file.c -o file && ./file
# 	fsl tests/thread.c -o thread && ./thread
	fsl tests/map.c -o map && ./map
	fsl tests/array.c -o array && ./array
	fsl tests/c_stdlib_fsl_support.c -o cstdlib_fsl && ./cstdlib_fsl
	gcc tests/c_stdlib_fsl_support.c -o cstdlib_fsl -lfsl && ./cstdlib_fsl
	fsl tests/change_heap.c -o change_heap && ./change_heap
	rm -rf heap file thread map array cstdlib_fsl change_heap
