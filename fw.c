#include <fsl.h>

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
    array       cons;
    int         con_count;

    conn_arr_t  whitlisted;
    conn_arr_t  blacklisted;

    /* Monitor Settings */
    int         running;
} firewall;

typedef firewall *firewall_t;
public firewall_t init_firewall(string ip, int port)
{
    firewall_t fw = allocate(0, sizeof(firewall));
    fw->watch_ip = str_dup(ip);
    fw->watch_port = port;
    fw->cons = init_array();
    fw->con_count = 0;

    fw->socket = allocate(0, sizeof(sock_t));
    fw->socket->fd = __syscall__(17, 3, _htons(0x0003), -1, -1, -1, _SYS_SOCKET);
    if(fw->socket->fd < 0)
        fsl_panic("unable to create socket!");

    mem_set(&fw->socket->addr, 0, sizeof(addr_in));
    fw->socket->buff_len = 1024;

    return fw;
}

public bool append_ip(firewall_t fw, conn_t con)
{
    if(!fw || !con)
        return -1;

    fw->cons = array_append(fw->cons, con);
    return 1;
}

public bool is_ip_whitlisted(firewall_t fw, string ip)
{
    if(!fw || !ip)
        return -1;

    for(int i = 0; i < fw->con_count; i++)
    {
        if(str_cmp(((connection **)fw->cons)[i]->dest_ip, ip))
            return i;
    }

    return -1;
}

public bool is_ip_blocked(firewall_t fw, string ip)
{
    if(!fw || !ip)
        return -1;

    for(int i = 0; i < fw->con_count; i++)
    {
        if(str_cmp(((connection **)fw->cons)[i]->dest_ip, ip))
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

    if(fw->cons)
        pfree_array((array)fw->cons);

    if(fw->whitlisted)
        pfree_array((array)fw->whitlisted);

    if(fw->blacklisted)
        pfree_array((array)fw->blacklisted);
}


void print_ip(unsigned int ip) {
    unsigned int h = _ntohl(ip);
    unsigned char *b = (unsigned char *)&h;

    // _printf("%d.%d.%d.%d", (void *)&b[0], (void *)&b[1], (void *)&b[2], (void *)&b[3]);
    
    printi(b[0]); print(".");
    printi(b[1]); print(".");
    printi(b[2]); print(".");
    printi(b[3]);
}

#define ETH_HDR_LEN sizeof(struct eth_hdr)
#define IP_HDR_LEN sizeof(struct ip_hdr)
#define TCP_HDR_LEN sizeof(struct tcp_hdr)

void parse(unsigned char *buf, size_t len) {
    if (len < 14) {
        println("Exit: 1");
        return;
    }

    unsigned short proto =
        (buf[12] << 8) | buf[13];

    print("\n=== Ethernet ===\n");

    if (proto != 0x0800) {
        println("Exit 2");
        return;
    }

    if (len < 14 + 20) {
        println("Exit 3");
        return;
    }

    unsigned char *ip = buf + 14;

    unsigned char ihl = ip[0] & 0x0F;
    int ip_header_len = ihl * 4;

    unsigned int saddr =
        (ip[12] << 24) | (ip[13] << 16) |
        (ip[14] << 8)  | (ip[15]);

    unsigned int daddr =
        (ip[16] << 24) | (ip[17] << 16) |
        (ip[18] << 8)  | (ip[19]);

    print("=== IP ===\nSrc: "), print_ip(saddr);
    print("\nDst: "), print_ip(daddr);
    print("\n");

    unsigned char protocol = ip[9];
    if(protocol == 1) {
        unsigned char *icmp = buf + 14 + ip_header_len;

        unsigned char type = icmp[0];
        unsigned char code = icmp[1];

        print("=== ICMP ===\n");

        print("Type: "), printi(type);
        print("| Code: "), printi(code), println(NULL);

        print("\n");
    } else if(protocol == 6) {
        unsigned char *tcp = buf + 14 + ip_header_len;

        unsigned short sport = (tcp[0] << 8) | tcp[1];
        unsigned short dport = (tcp[2] << 8) | tcp[3];
        unsigned char flags = tcp[13];

        print("=== TCP ===\nSrc Port: "), printi(sport), print("| Dst Port: "), printi(dport), println(NULL);

        int syn = (flags & 0x02) != 0;
        int ack = (flags & 0x10) != 0;
        int fin = (flags & 0x01) != 0;
        _printf("Flags: SYN = %d | ACK = %d | FIN = %d\n", (void *)&syn, (void *)&ack, (void *)&fin);
    }
    else if(protocol == 17) {
        unsigned char *udp = buf + 14 + ip_header_len;

        unsigned short sport = (udp[0] << 8) | udp[1];
        unsigned short dport = (udp[2] << 8) | udp[3];

        println("=== UDP ===\n");
        print("Src Port: "), printi(sport), print("| Dst Port: "), printi(dport), println(NULL);
    }
}

public fn monitor(firewall_t fw)
{
    fw->running = 1;

    string data = _EXTERNAL_;
    while(fw->running != 0)
    {
        // string data = sock_read(fw->socket);
        // if(!data) {
        //     println("Skipping...");
        //     continue;
        // }

        // int sz = __get_size__(data);
        int sz = 1024;
        int bytes = __syscall__(fw->socket->fd, (long)data, 1023, -1, -1, -1, _SYS_READ);

        _printf("\x1b[32mNew Request, Byte Size: %d\x1b[39m\n", (void *)&bytes);
        char byte[4];
        for (size_t i = 0; i < bytes; i++) {
            byte_to_hex(data[i], byte); 
            _printf("%s ", byte);
        }

        println("\n\n\x1b[32mRequest Info\x1b[39m");

        parse(data, bytes);
        println(NULL);
        // pfree(data, 1);
    }
}

public int entry(int argc, string argv[])
{
    // toggle_debug_mode();
    uninit_mem();
    set_heap_sz(_HEAP_PAGE_ * 10);
    init_mem();
    firewall_t fw = init_firewall("1.1.1.1", 80);
    _printf("Socket: %d\n", (void *)&fw->socket->fd);

    monitor(fw);
    return 0;
}