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
			free(ptr->name);
			free(ptr);
			break ;
		}
		last = ptr;
		ptr = ptr->next;
	}
	output(client_fd, "See ya coward !\n");
	close(client_fd);
	printf("[Closed connection] %d disconnected\n", client_fd);
	pthread_exit((void *)0);
}

void	display_channel(t_client *client, int fd)
{
	t_client	*head;

	head = get_client(NULL);
	dprintf(fd, "Chan name : %s\nUsers connected here :\n", client->channel);
	while (head)
	{
		if (strcmp(head->channel, client->channel) == 0)
			dprintf(fd, "%s\n", head->name);
		head = head->next;
	}
}

void	change_channel(t_client *client, char *chan, int fd)
{
	if (strcmp(chan, "general") == 0 && strchr(client->name, '*') == 0)
	{
		printf("[Incorrect access try] non authentificated user %s tried to join general\n", client->name);
		output(fd, "You are not authorised\n");
		close_client(fd);
	}
	print_in_channel(client->name, client->channel);
	print_in_channel(" quitted ", client->channel);
	print_in_channel(client->channel, client->channel);
	print_in_channel("\n", client->channel);
	client->channel = chan;
	print_in_channel(client->name, chan);
	print_in_channel(" joined ", chan);
	print_in_channel(chan, chan);
	print_in_channel("\n", chan);
}

void	print_in_channel(char *msg, char *channel)
{
	t_client	*head;

	head = get_client(NULL);
	while (head)
	{
		if (strcmp(channel, head->channel) == 0)
			dprintf(head->fd, "%s", msg);
		head = head->next;
	}
}

void	msg_in_channel(char *msg, char *channel, char *name)
{
	t_client	*head;

	head = get_client(NULL);
	while (head)
	{
		if (head->fd == 0)
			printf("[New message](%s%s%s) %s> %s\n", RED, channel, DEF, name, msg);
		else if (strcmp(channel, head->channel) == 0 && strcmp(name, head->name) != 0)
			dprintf(head->fd, "%s> %s\n", name, msg);
		head = head->next;
	}
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
