#include "irc_server.h"

void	close_client(int client_fd)
{
	t_client	*ptr;
	t_client	*last;

	ptr = get_client(NULL);
	while (ptr)
	{
		if (ptr->fd == client_fd)
		{
			last->next = ptr->next;
			print_in_channel(ptr->name, ptr->channel);
			print_in_channel(" left the channel", ptr->channel);
			ft_bzero(ptr, sizeof(t_client));
			free(ptr);
			break ;
		}
		last = ptr;
		ptr = ptr->next;
	}
	output_error(client_fd, "See ya coward !\n");
	close(client_fd);
	printf("[Closed connection] %d disconnected\n", client_fd);
	pthread_exit((void *)0);
}

void	add_client(t_client	*client)
{
	t_client				*last;
	static int				access = 1;

	while (access == 0)
		;
	access = 0;
	last = get_client(NULL);
	if (!last)
		get_client(client);
	while (last->next != NULL)
		last = last->next;
	last->next = client;
	access = 1;
}

t_client	*get_client(t_client *client)
{
	static t_client	*memory = NULL;

	if (client != NULL)
		memory = client;
	return (memory);
}
