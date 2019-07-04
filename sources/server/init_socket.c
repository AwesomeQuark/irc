#include "irc_server.h"

void	fill_capacity(struct sockaddr_in *capa, int port)
{
	capa->sin_family		=	AF_INET;
	capa->sin_port			=	htons(port);
	capa->sin_addr.s_addr	=	INADDR_ANY;
}

int		server_init_socket(int port)
{
	struct protoent		*communication_protocol = NULL;
	struct sockaddr_in	socket_capacity;
	int					socket_fd;

	fill_capacity(&socket_capacity, port);
	PROT(communication_protocol = getprotobyname("tcp"), -1, "getprotobyname")
	PROT(socket_fd = socket(PF_INET, SOCK_STREAM, communication_protocol->p_proto), -1, "socket")
	PROT(bind(socket_fd, (struct sockaddr*)&socket_capacity, sizeof(socket_capacity)), -1, "bind")
	PROT(listen(socket_fd, MAX_CONNECTIONS), -1, "listen")
	get_socket_fd(socket_fd);
	return (socket_fd);
}
