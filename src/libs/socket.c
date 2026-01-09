#include "../../headers/clibp.h"

sock_t listen_tcp(const string ip, int port, int concurrent)
{
	if(port <= 0)
		return NULL;

    long sock = __syscall__(AF_INET, 1, 0, 0, 0, 0, _SYS_SOCKET);
    if (sock < 0)
		clibp_panic("error, unable to create a socket...!");

	if(__CLIBP_DEBUG__)
		print("Socket successfully created: "), _printi(sock), print("\n");

    /* reuse address */
    int reuse = 1;
    long cc = ___syscall__(sock, SOL_SOCKET, SO_REUSEADDR, (long)&reuse, sizeof(reuse), 0, _SYS_SETSOCKOPT);
    if (cc < 0)
		clibp_panic("error, unable to reuse addr...!");

    /* bind */
    _sockaddr_in addr;
    mem_set(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = _htons((unsigned short)port);
    addr.sin_addr.s_addr = 0; // INADDR_ANY

    long ret = __syscall__(sock, (long)&addr, sizeof(addr), 0, 0, 0, _SYS_BIND);
    if (ret < 0)
		clibp_panic("error, unable to bind socket...!");

    /* listen */
    ret = __syscall__(sock, concurrent, 0, 0, 0, 0, _SYS_LISTEN);

	sock_t socket = allocate(0, sizeof(_sock_t));
	socket->fd = sock;
	socket->addr = addr;
	return socket;
}

sock_t sock_accept(sock_t server, len_t len)
{
	#if defined(___x86___) || defined(__riscv)
		long client_fd = __syscall__(server->fd, 0, 0, -1, -1, -1, _SYS_ACCEPT4);
	#elif defined(__x86_64__)
		long client_fd = __syscall__(server->fd, 0, 0, -1, -1, -1, _SYS_ACCEPT);
	#endif
	
	if(client_fd < 0)
		return NULL;

	sock_t client = allocate(0, sizeof(_sock_t));
	client->fd = client_fd;
	client->buff_len = len;

	return client;
}

int sock_write(sock_t sock, string buffer)
{
	return __syscall__(sock->fd, (long)buffer, str_len(buffer), -1, -1, -1, _SYS_WRITE);
}

string sock_read(sock_t sock)
{
	char BUFF[sock->buff_len];
	long bytes = __syscall__(sock->fd, (long)BUFF, sock->buff_len, 0, 0, 0, _SYS_READ);
	if(bytes > 0)
	{
		string buffer = allocate(0, bytes + 1);
		mem_cpy(buffer, BUFF, sock->buff_len);
		buffer[bytes] = '\0';
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

fn sock_close(sock_t sock)
{
	__syscall__(sock->fd, -1, -1, -1, -1, -1, _SYS_CLOSE);
	mem_set(sock, 0, sizeof(_sock_t));
}
