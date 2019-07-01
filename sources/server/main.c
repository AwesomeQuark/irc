#include "irc_server.h"

void	server_quit(int status)
{
	PROT(close(get_socket_fd(0)), -1, "close");
	exit(status);
}

int		main(int ac, char **av)
{
	int				socket_fd;

	if (ac != 2)
	{
		USAGE_SERVER;
		return (SUCCESS);
	}
	signal(SIGINT, server_quit);
	socket_fd = server_init_socket(atoi(av[1]));
	server_listen(socket_fd);
}
