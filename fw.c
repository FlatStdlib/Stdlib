#include <fsl.h>

#define HEAP_USED

struct pollfd {
    int fd;
    short events;
    short revents;
};

#define POLLIN 0x001

typedef struct
{
    string      src_ip;
    int         src_port;
    string      dest_ip;
    int         dest_port;
} connection;

typedef connection *conn_t;
typedef connection **conn_arr_t;
typedef struct 
{
    /* Raw Socket Info */
    sock_t      socket;

    /* Protection */
    string      watch_ip;
    int         watch_port;

    /* Monitor Stats */
    array       whitlisted;
    array       blacklisted;

    /* Monitor Settings */
    int         running;
} firewall;

typedef firewall *firewall_t;
public firewall_t init_firewall(string ip, int port)
{
    firewall_t fw = allocate(0, sizeof(firewall));
    fw->watch_ip = str_dup(ip);
    fw->watch_port = port;

    fw->blacklisted = init_array();
    fw->whitlisted = init_array();

    fw->socket = allocate(0, sizeof(sock_t));
    fw->socket->fd = __syscall__(17, 3, _htons(0x0003), -1, -1, -1, _SYS_SOCKET);
    if(fw->socket->fd < 0)
        fsl_panic("unable to create socket!");

    mem_set(&fw->socket->addr, 0, sizeof(addr_in));
    fw->socket->buff_len = 1024;

    return fw;
}

public bool whitlist_ip(firewall_t fw, string ip)
{
    if(!fw || !ip)
        return 0;

    fw->whitlisted = array_append(fw->whitlisted, ip);
    return 1;
}

public bool blacklist_ip(firewall_t fw, string ip)
{
    if(!fw || !ip)
        return 0;

    fw->blacklisted = array_append(fw->blacklisted, ip);
    return 1;
}

/* 
    Check if an IP Address is already whitlisted 

    NOTE: IPs here can never be blocked during an attack
*/
public bool is_ip_whitlisted(firewall_t fw, string ip)
{
    if(!fw || !ip)
        return -1;

    for(int i = 0; i < __get_size__(fw->whitlisted); i++)
    {
        if(!((string *)fw->whitlisted)[i])
            break;

        if(str_cmp(((string *)fw->whitlisted)[i], ip))
            return i;
    }

    return -1;
}
/* 
    Check if an IP Address is already blacklisted
*/
public bool is_ip_blacklisted(firewall_t fw, string ip)
{
    if(!fw || !ip)
        return -1;

    for(int i = 0; i < __get_size__(fw->blacklisted); i++)
    {
        if(!((string *)fw->blacklisted)[i])
            break;

        if(str_cmp(((string *)fw->blacklisted)[i], ip))
            return i;
    }

    return -1;
}

public fn firewall_destruct(firewall_t fw)
{
    if(!fw)
        return;

    __syscall__(fw->socket->fd, -1, -1, -1, -1, -1, _SYS_CLOSE);

    pfree(fw->watch_ip, 1);

    if(fw->whitlisted)
        pfree_array((array)fw->whitlisted);

    if(fw->blacklisted)
        pfree_array((array)fw->blacklisted);
}

HEAP_USED
public string ip_to_str(unsigned int ip) {
    unsigned int h = _ntohl(ip);
    unsigned char *b = (unsigned char *)&h;
    
    string n = allocate(0, 16);
    str_append_int(n, b[0]);
    str_append(n, ".");
    str_append_int(n, b[1]);
    str_append(n, ".");
    str_append_int(n, b[2]);
    str_append(n, ".");
    str_append_int(n, b[3]);

    return n;
}

public fn print_ip(unsigned int ip) {
    unsigned int h = _ntohl(ip);
    unsigned char *b = (unsigned char *)&h;

    printi(b[0]); print(".");
    printi(b[1]); print(".");
    printi(b[2]); print(".");
    printi(b[3]);
}

#define ETH_HDR_LEN sizeof(struct eth_hdr)
#define IP_HDR_LEN sizeof(struct ip_hdr)
#define TCP_HDR_LEN sizeof(struct tcp_hdr)

HEAP_USED
public bool parse(firewall_t fw, unsigned char *buf, len_t len) {
    if (len < 14) return 0;

    unsigned short proto = (buf[12] << 8) | buf[13];

    if (proto != 0x0800) return 0;
    if (len < 14 + 20) return 0;

    unsigned char *ip = buf + 14;

    unsigned char ihl = ip[0] & 0x0F;
    int ip_header_len = ihl * 4;

    unsigned int saddr =
        (ip[12] << 24) | (ip[13] << 16) |
        (ip[14] << 8)  | (ip[15]);

    unsigned int daddr =
        (ip[16] << 24) | (ip[17] << 16) |
        (ip[18] << 8)  | (ip[19]);

    string sip = ip_to_str(saddr);
    string dip = ip_to_str(daddr);

    if(!sip || !dip)
        return 0;

    if(is_ip_whitlisted(fw, sip) > -1)
        return 0;

    if(is_ip_whitlisted(fw, dip) > -1)
        return 0;

    pfree(sip, 1);
    pfree(dip, 1);

    _printf("\x1b[32mNew Request, Byte Size: %d\x1b[39m\n", (void *)&len);
    print("=== IP ===\nSrc: "), print_ip(saddr);
    print("\nDst: "), print_ip(daddr);
    print("\n");

    unsigned char protocol = ip[9];
    if(protocol == 1) {
        unsigned char *icmp = buf + 14 + ip_header_len;

        unsigned char type = icmp[0];
        unsigned char code = icmp[1];

        print("\x1b[33m=== ICMP ===\x1b[39m\n");

        print("Type: "), printi(type);
        print(" | Code: "), printi(code), println(NULL);

        print("\n");
    } else if(protocol == 6) {
        unsigned char *tcp = buf + 14 + ip_header_len;

        unsigned short sport = (tcp[0] << 8) | tcp[1];
        unsigned short dport = (tcp[2] << 8) | tcp[3];
        unsigned char flags = tcp[13];

        print("\x1b[31m=== TCP ===\x1b[39m\nSrc Port: "), printi(sport), print(" | Dst Port: "), printi(dport), println(NULL);

        int syn = (flags & 0x02) != 0;
        int ack = (flags & 0x10) != 0;
        int fin = (flags & 0x01) != 0;
        _printf("Flags: SYN = %d | ACK = %d | FIN = %d\n", (void *)&syn, (void *)&ack, (void *)&fin);
    }
    else if(protocol == 17) {
        unsigned char *udp = buf + 14 + ip_header_len;

        unsigned short sport = (udp[0] << 8) | udp[1];
        unsigned short dport = (udp[2] << 8) | udp[3];

        println("\x1b[32m=== UDP ===\x1b[39m\n");
        print("Src Port: "), printi(sport), print(" | Dst Port: "), printi(dport), println(NULL);
    }

    char byte[4];
    println("\x1b[32mRequest Info\x1b[39m");
    for (size_t i = 0; i < len; i++) {
        byte_to_hex(buf[i], byte); 
        _printf("%s ", byte);
    }
    println("\n");

    return 1;
}

public fn monitor(firewall_t fw)
{
    fw->running = 1;

    string data = _EXTERNAL_;
    
    while(fw->running != 0)
    {
        int sz = 1024;
        int bytes = __syscall__(fw->socket->fd, (long)data, 1023, -1, -1, -1, _SYS_READ);

        if(bytes > 0) {
            parse(fw, data, bytes);
        }
    }
}

char arg_ip[1024];

public int entry(int argc, string argv[])
{
    int len = 0;
    if(argc > 1) {
        len = str_len(argv[1]);
        mem_cpy(arg_ip, argv[1], len);
    }

    uninit_mem();
    set_heap_sz(526870912);
    init_mem();
    firewall_t fw = init_firewall("1.1.1.1", 80);
    _printf("Socket: %d\n", (void *)&fw->socket->fd);

    if(len > 2)
        whitlist_ip(fw, arg_ip);

    monitor(fw);
    return 0;
}