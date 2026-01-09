#include <clibp.h>

int entry()
{
	__CLIBP_DEBUG__ = 1;
	_sock_t server = listen_tcp(NULL, 50, 999);
	if(server.fd < 0) println("FAILED\n");

	_sock_t client = sock_accept(server, 1024);

	sock_write(client, "hi");
	sock_close(client);
	return 0;
}
