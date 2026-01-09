#include <clibp.h>

int entry()
{
	__CLIBP_DEBUG__ = 1;
	sock_t server = listen_tcp(NULL, 50, 999);
	if(server->fd < 0) println("FAILED\n");

	println("Listening....");
	sock_t client;
	while(1)
	{
		if(!(client = sock_accept(server, 1024)))
			continue;
		println("Client Connected");
	}

	sock_write(client, "hi");
	sock_close(client);
	return 0;
}
