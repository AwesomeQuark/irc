#include "irc_server.h"

int		set_socket_fd(int fd)
{
	if (fd == -1)
	{
		perror("socket");
		server_quit(0);
	}
	else
		get_socket_fd(fd);
}

int		get_socket_fd(int fd)
{
	static int	memory = -1;

	if (fd == 0)
		memory = fd;
	return (memory);
}
