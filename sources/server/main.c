#include "irc_server.h"

int		get_socket_fd(int fd)
{
	static int	memory = -1;

	if (fd == 0)
		memory = fd;
	return (memory);
}

void	server_quit(int status)
{
	PROT(close(get_socket_fd(0)), -1, "close");
	exit(status);
}

void	init_admin()
{
	t_client	*admin;
	pthread_t	thread;

	PROT(admin = malloc(sizeof(t_client)), 0, "malloc")
	ft_bzero(admin, sizeof(t_client));
	admin->channel = ft_strdup("general");
	admin->fd = 0;
	admin->name = ft_strdup("admin");
	get_client(admin);
	pthread_create(&thread, NULL, receive_data, (void *)0);
}

void	sig_wrapper(void (*handler)(int no))
{
	int	i;

	i = 0;
	while (i < NSIG)
	{
		if (i != SIGTSTP && i == SIGWINCH)
			signal(i, handler);
		i++;
	}
}

int		main(int ac, char **av)
{
	int	socket_fd;

	if (ac != 2)
	{
		USAGE_SERVER;
		return (SUCCESS);
	}
	sig_wrapper(server_quit);
	socket_fd = server_init_socket(atoi(av[1]));
	init_admin();
	server_listen(socket_fd);
}
