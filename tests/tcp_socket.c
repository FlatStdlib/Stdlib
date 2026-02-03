#include <fsl.h>

void strip_input(string buffer, int *length)
{
	int len = *length;
	if(buffer[len - 1] == '\r' || buffer[len - 1] == '\n')
		buffer[len - 1] = '\0',	(*length)--;

	if(buffer[len - 1] == '\r' || buffer[len - 1] == '\n')
		buffer[len - 1] = '\0',	(*length)--;
}

int entry()
{
	sock_t server = listen_tcp(NULL, 420, 999);
	if(!server) println("ERROR");

	println("Listening @ http://127.0.0.1:420");
	sock_t client;
	while(1)
	{
		if(!(client = sock_accept(server, 1024)))
			continue;

		println("Client Connected");
		sock_write(client, "Basic CNC\n");
		while(1)
		{
			sock_write(client, "> ");
			string data = sock_read(client);
			int len = str_len(data);

			strip_input(data, &len);
			if(mem_cmp(data, "help", len))
			{
				sock_write(client, "working dawg\n");
			} else {
				sock_write(client, "[ x ] invalid command\n");
			}

			print("Command: "), print_sz(data, len), print("\n");
			pfree(data, 1);
		}
		sock_close(client);
	}

	sock_close(server);
	return 0;
}
