#include "irc_client.h"

void	fill_capacity(struct sockaddr_in *capa, int port)
{
	capa->sin_family		= AF_INET;
	capa->sin_port			= htons(port);
	capa->sin_addr.s_addr	= INADDR_ANY;
}

int		client_init_socket(int port)
{
	struct protoent		*communication_protocol = NULL;
	struct sockaddr_in	socket_capacity;
	int					socket_fd = 0;

	fill_capacity(&socket_capacity, port);
	PROT(communication_protocol = getprotobyname("tcp"), -1, "getprotobyname")
	PROT(socket_fd = socket(PF_INET, SOCK_STREAM, communication_protocol->p_proto), -1, "socket")
	PROT(connect(socket_fd, (struct sockaddr*)&socket_capacity, sizeof(socket_capacity)), -1, "bind")
	return (socket_fd);
}

void	client_talk(int fd)
{
	char	line[65];

	printf("Initialised");
	while (1)
	{
		ft_bzero(line, 65);
		read(0, line, 64);
		send(fd, line, ft_strlen(line), 0);
		printf("### sent ###\n");
	}
}

int		main(int ac, char **av)
{
	int		socket_fd;

	if (ac != 3)
	{
		printf(USAGE_CLIENT);
		return (FAILURE);
	}
	socket_fd = client_init_socket(atoi(av[1]));
	client_talk(socket_fd);
}
