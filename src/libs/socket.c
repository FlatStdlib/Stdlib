#include "../../headers/clibp.h"

_sock_t listen_tcp(const string ip, int port, int concurrent)
{
	if(!ip || port <= 0)
		return (_sock_t){0};

	long sock = __syscall__(2, 1, 0, 0, 0, 0, _SYS_SOCKET);
	if(sock < 0) {
		println("[ x ] Error, Unable to create socket...!");
		return (_sock_t){-1};
	}

	sockaddr_in server_addr, client_addr;

	server_addr.sin_family = 0;
	if(ip && !parse_ipv4(ip, &server_addr.sin_addr))
	{
		println("[ x ] Error, Invalid IP for socket...!");
		return (_sock_t){-1};
	} else {
		server_addr.sin_addr = 0;
	}

	server_addr.sin_port = _htons(port);

	int reuse = 1;
	long cc = __syscall__(sock, 1, 2, (long)&reuse, sizeof(reuse), 0, _SYS_SETSOCKOPT);
	if(cc < 0)
	{
		println("[ x ] Error, Unable to set socket option...!");
		return (_sock_t){-1};
	}

	long chk = __syscall__(sock, (long)&server_addr, sizeof(server_addr), 0, 0, 0, _SYS_BIND);
	if(chk < 0)
	{
		println("[ x ] Error, Unable to bind socket...!");
		return (_sock_t){-1};
	}

	long c = __syscall__(sock, concurrent, 0, 0, 0, 0, _SYS_LISTEN);
	if(c < 0)
	{
		println("[ x ] Error, Unable to listen to socket....!");
		return (_sock_t){-1};
	}

	_sock_t socket = {
		.fd = sock,
		.addr = server_addr
	};

	return socket;
}

_sock_t sock_accept(_sock_t sock, len_t len)
{
	#if defined(___x86___) || defined(__riscv)
		long client_fd = __syscall__(sock.fd, 0, 0, -1, -1, -1, _SYS_ACCEPT4);
	#elif defined(__x86_64__)
		long client_fd = __syscall__(sock.fd, 0, 0, -1, -1, -1, _SYS_ACCEPT);
	#endif
	_sock_t s = {
		.fd = client_fd,
		.buff_len = len
	};

	return s;
}

string sock_read(_sock_t sock)
{
	char BUFF[sock.buff_len];
	long bytes = __syscall__(sock.fd, (long)BUFF, sock.buff_len, 0, 0, 0, _SYS_READ);
	if(bytes > 0)
	{
		string buffer = allocate(0, bytes);
		mem_cpy(buffer, BUFF, sock.buff_len);
		return buffer;
	}

	return NULL;
}

int create_socket(int family, int type, int protocol)
{
	return __syscall__(family, type, protocol, 0, 0, 0, _SYS_SOCKET);
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
