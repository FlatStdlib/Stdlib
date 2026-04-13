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
    fw->socket->fd = __syscall__(17, 3, 0x0003, -1, -1, -1, _SYS_SOCKET);
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

struct eth_hdr {
    unsigned char dst[6];
    unsigned char src[6];
    unsigned short proto;
};

// Minimal IPv4 header
struct ip_hdr {
    unsigned char ihl_version;
    unsigned char tos;
    unsigned short tot_len;
    unsigned short id;
    unsigned short frag_off;
    unsigned char ttl;
    unsigned char protocol;
    unsigned short check;
    unsigned int saddr;
    unsigned int daddr;
};

// Minimal TCP header
struct tcp_hdr {
    unsigned short src_port;
    unsigned short dst_port;
    unsigned int seq;
    unsigned int ack;
    unsigned char offset_reserved;
    unsigned char flags;
    unsigned short window;
    unsigned short checksum;
    unsigned short urgent;
};

// Minimal UDP header
struct udp_hdr {
    unsigned short src_port;
    unsigned short dst_port;
    unsigned short len;
    unsigned short check;
};

void print_ip(unsigned int ip) {
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

void parse(unsigned char *buf, size_t len) {
    if (len < ETH_HDR_LEN) return;

    struct eth_hdr *eth = (struct eth_hdr *)buf;
    unsigned short proto = _ntohs(eth->proto);

    print("\n=== Ethernet ===\n");

    if (proto != 0x0800) return;

    if (len < ETH_HDR_LEN + IP_HDR_LEN) return;

    struct ip_hdr *ip = (struct ip_hdr *)(buf + ETH_HDR_LEN);
    int ip_header_len = (ip->ihl_version & 0x0F) * 4;

    print("=== IP ===\n");

    print("Src: ");
    print_ip(ip->saddr);
    print("\nDst: ");
    print_ip(ip->daddr);
    print("\n");

    // TCP
    if (ip->protocol == 6) {
        if (len < ETH_HDR_LEN + ip_header_len + TCP_HDR_LEN) return;

        struct tcp_hdr *tcp = (struct tcp_hdr *)(buf + ETH_HDR_LEN + ip_header_len);

        print("=== TCP ===\n");

        print("Src Port: ");
        printi(_ntohs(tcp->src_port));
        print("\nDst Port: ");
        printi(_ntohs(tcp->dst_port));
        print("\n");

        unsigned char flags = tcp->flags;

        print("Flags: SYN=");
        printi((flags & 0x02) != 0);
        print(" ACK=");
        printi((flags & 0x10) != 0);
        print(" FIN=");
        printi((flags & 0x01) != 0);
        print("\n");
    }

    // UDP
    else if (ip->protocol == 17) {
        struct udp_hdr *udp = (struct udp_hdr *)(buf + ETH_HDR_LEN + ip_header_len);

        print("=== UDP ===\n");

        print("Src Port: ");
        printi(_ntohs(udp->src_port));
        print("\nDst Port: ");
        printi(_ntohs(udp->dst_port));
        print("\n");
    }
}

public fn monitor(firewall_t fw)
{
    fw->running = 1;

    while(fw->running != 0)
    {
        string data = sock_read(fw->socket);
        if(!data)
            continue;

        println(data);
        parse(data, __get_size__(data));
    }
}

public int entry(int argc, string argv[])
{
    firewall_t fw = init_firewall("1.1.1.1", 80);
    _printf("Socket: %d\n", (void *)&fw->socket->fd);

    monitor(fw);
    return 0;
}