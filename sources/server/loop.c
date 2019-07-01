#include "irc_server.h"

void	close_client(int *client_fd, int i)
{
	close(client_fd[i]);
	printf("fd %d disconnected\n", client_fd[i]);
	client_fd[i] = 0;
}

void	receive_data(int *client_fd, fd_set *set)
{
	char	line[64];

	for (int i = 0; i < MAX_CONNECTIONS; i++)
	{
		if (FD_ISSET(client_fd[i], set))
		{
			if (recv(client_fd[i], (char *)&line, 63, 0) == 0)
				close_client(client_fd, i);
			ft_putstr_fd((char *)&line, 1);
			ft_bzero((char *)&line, 64);
		}
	}
}

void	new_connection(int *client_fd, int socket_fd, fd_set *set)
{
	socklen_t			size;
	struct sockaddr_in	addr;
	int					new_fd;

	size = sizeof(addr);
	PROT(new_fd = accept(socket_fd, (struct sockaddr *)&addr, &size), -1, "connect");
	printf("[New connection] socket fd: %s%d%s, ip: %s%s%s, port: %s%d%s \n", RED, new_fd, DEF, GREEN, inet_ntoa(addr.sin_addr), DEF, YELLOW, ntohs(addr.sin_port), DEF);
	for (int i = 0; i < MAX_CONNECTIONS; i++)
		if (client_fd[i] == 0)
		{
			client_fd[i] = new_fd;
			FD_SET(new_fd, set);
			send(new_fd, "Welcome to HAF irc server :)\n", 30, 0);
			break ;
		}
}

void	init_fd_set(fd_set *set, int socket_fd)
{
	FD_ZERO(set);
	FD_SET(socket_fd, set);
	for (int i = 1; i < MAX_CONNECTIONS; i++)
		FD_SET(i, set);
}

void	server_listen(int socket_fd)
{
	int					client_fd[MAX_CONNECTIONS];
	fd_set				set;

	for (int i = 0; i < MAX_CONNECTIONS; i++)
		client_fd[i] = 0;
	while (1)
	{
		init_fd_set(&set, socket_fd);
		PROT(select(socket_fd + 1, &set, NULL, NULL, NULL), -1, "select")
		if (FD_ISSET(socket_fd, &set))
			new_connection((int *)&client_fd, socket_fd, &set);
	//	else
		receive_data((int *)&client_fd, &set);
	}
}
