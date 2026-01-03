#include "../../headers/clibp.h"

_sock_t listen_tcp(const str ip, int port, int concurrent)
{
	if(!ip || port <= 0)
		return (_sock_t){0};

	__syscall(_SYS_SOCKET, 2, 1, 0, 0, 0, 0);
	register int sck asm("rax");
	int sock = sck;
	if(sock < 0)
	{
		print("[ x ] Error,q Unable to create socket...!\n");
		__exit(1);
	}

	sockaddr_in server_addr, client_addr;

	server_addr.sin_family = 0;
	if(ip && !parse_ipv4(ip, &server_addr.sin_addr))
	{
		print("[ x ] Error, Invalid IP for socket...!\n");
		__exit(1);
	} else {
		server_addr.sin_addr = 0;
	}

	server_addr.sin_port = _htons(port);

	int reuse = 1;
	__syscall(_SYS_SETSOCKOPT, sock, 1, 2, (long)&reuse, sizeof(reuse), 0);
	register int cc asm("rax");
	if(cc < 0)
	{
		print("[ x ] Error, Unable to set socket option...!\n");
		__exit(1);
	}

	__syscall(49, sock, (long)&server_addr, sizeof(server_addr), 0, 0, 0);
	register int chk asm("rax");
	if(chk < 0)
	{
		print("[ x ] Error, Unable to bind socket...!\n");
		__exit(1);
	}

	__syscall(_SYS_LISTEN, sock, concurrent, 0, 0, 0, 0);
	register int c asm("rax");
	if(c < 0)
	{
		print("[ x ] Error, Unable to listen to socket....!\n");
		__exit(1);
	}

	_sock_t socket = {
		.fd = sock,
		.addr = server_addr
	};

	return socket;
}

_sock_t sock_accept(_sock_t sock, len_t len)
{
	#if defined(___x86___)
		__syscall(_SYS_ACCEPT4, sock.fd, 0, 0, -1, -1, -1);
	#elif defined(___x86_64___)
		__syscall(_SYS_ACCEPT, sock.fd, 0, 0, -1, -1, -1);
	#endif
	register int client_fd asm("rax");
	_sock_t s = {
		.fd = client_fd,
		.buff_len = len
	};

	return s;
}

str sock_read(_sock_t sock)
{
	char BUFF[sock.buff_len];
	__syscall(_SYS_READ, sock.fd, (long)BUFF, sock.buff_len, 0, 0, 0);
	register int bts asm("rax");
	int bytes = bts;
	if(bytes > 0)
	{
		str buffer = allocate(0, bytes);
		mem_cpy(buffer, BUFF, sock.buff_len);
		return buffer;
	}

	return NULL;
}

int create_socket(int family, int type, int protocol)
{
	__syscall(family, type, protocol, 0, 0, 0, 0);
	register int sock asm("rax");
	return sock;
}

int parse_ipv4(const char *ip, unsigned int *out)
{
    unsigned int res = 0;
    int octet = 0;
    int shift = 24;
    int dots = 0;

    if (!ip || !out)
        return 0;

    while (*ip)
    {
        if (*ip >= '0' && *ip <= '9')
        {
            octet = octet * 10 + (*ip - '0');
            if (octet > 255)
                return 0;
        }
        else if (*ip == '.')
        {
            if (dots >= 3 || shift < 0)
                return 0;
            res |= (octet << shift);
            shift -= 8;
            octet = 0;
            dots++;
        }
        else
        {
            return 0;
        }
        ip++;
    }

    if (dots != 3)
        return 0;

    res |= octet;
    *out = res;
    return 1;
}


char *convert_ip(unsigned int ip) {
    static char buf[16];

    unsigned char b1 = (ip >> 24) & 0xFF;
    unsigned char b2 = (ip >> 16) & 0xFF;
    unsigned char b3 = (ip >> 8)  & 0xFF;
    unsigned char b4 = ip & 0xFF;

    __sprintf(buf, "%u.%u.%u.%u", &b1, &b2, &b3, &b4);
    return buf;
}

unsigned short _htons(unsigned short x)
{
	return ((x & 0xFF) << 8) | ((x & 0xFF00) >> 8);
}

unsigned int _htonl(unsigned int x)
{
    return ((x & 0x000000FF) << 24) |
           ((x & 0x0000FF00) << 8)  |
           ((x & 0x00FF0000) >> 8)  |
           ((x & 0xFF000000) >> 24);
}
