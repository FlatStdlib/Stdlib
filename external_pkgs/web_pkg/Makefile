.PHONY: all

all: compile_w_ssl

compile_w_ssl:
	gcc -ffreestanding \
	web.c src/*.c \
	/usr/lib/libclibp.a \
	-lssl -lcrypto

# Bloatware
NOSSL:
	gclibp --output a.out \
	web.c \
	src/webserver.c \
	src/parser.c \
	src/router.c \
	src/utils.c 