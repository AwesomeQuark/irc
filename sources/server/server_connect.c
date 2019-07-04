#include "irc_server.h"

void	new_connection(int socket_fd)
{
	socklen_t			size;
	struct sockaddr_in	addr;
	int					new_fd;
	pthread_t			thread;

	size = sizeof(addr);
	new_fd = accept(socket_fd, (struct sockaddr *)&addr, &size);
	printf("[New connection] socket fd: %s%d%s, ip: %s%s%s, port: %s%d%s \n", RED, new_fd, DEF, GREEN, inet_ntoa(addr.sin_addr), DEF, YELLOW, ntohs(addr.sin_port), DEF);
	pthread_create(&thread, NULL, receive_data, (void *)new_fd);
}

void	server_listen(int socket_fd)
{
	int		conn_nb;
	fd_set	set;

	while (1)
	{
		FD_ZERO(&set);
		FD_SET(socket_fd, &set);
		PROT(conn_nb = select(socket_fd + 1, &set, NULL, NULL, NULL), -1, "select")
		while (conn_nb--)
			new_connection(socket_fd);
	}
}
