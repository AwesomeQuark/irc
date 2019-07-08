#include "irc_server.h"

void	display_channel(t_client *client, int fd)
{
	t_client	*head = get_client(NULL);
	char		*tmp = NULL;

	sprintf(tmp, "%sChan name : %s%s%s\nUsers connected here :%s\n", GREEN, RED, client->channel, GREEN, BLUE);
	output(fd, tmp);
	while (head)
	{
		if (strcmp(head->channel, client->channel) == 0)
			dprintf(fd, "%s\n", head->name);
		head = head->next;
	}
	output(fd, "\033[0m");
}

void	change_channel_message(t_client *client, char *chan)
{
	print_in_channel("\033[32m", client->channel);
	print_in_channel(client->name, client->channel);
	print_in_channel(" quitted ", client->channel);
	print_in_channel(client->channel, client->channel);
	print_in_channel("\n", client->channel);
	strcpy(client->channel, chan);
	print_in_channel(client->name, chan);
	print_in_channel(" joined ", chan);
	print_in_channel(chan, chan);
	print_in_channel("\n", chan);
	print_in_channel("\033[0m", client->channel);
	clear_str(chan);
}

void	change_channel(t_client *client, char *chan, int fd)
{
	if (strcmp(chan, "general") == 0 && strchr(client->name, '*') == 0)
	{
		output_error(fd, "You are not authorised\n");
		close_client(fd);
	}
	if (strchr(chan, '#') || strchr(chan, '%') || strchr(chan, ' '))
	{
		output_error(fd, "Unauthorised char [#% ]\n");
		return ;
	}
	if (strlen(chan) > 31)
	{
		output_error(fd, "The name cannot be more than 31 characters\n");
		return ;
	}
	change_channel_message(client, chan);
}

void	private_channel(t_client *client, char *target_name)
{
	t_client	*head = get_client(NULL);
	char		*chan_name;

	if (client->name[0] != '*')
	{
		output_error(client->fd, "you have to be an authentified user, sorry mate\n");
		clear_str(target_name);
		return ;
	}
	head = head->next;
	while (head)
	{
		if (strcmp(head->name, target_name) == 0)
		{
			chan_name = concat(client->name, "#", head->name);
			strcpy(client->channel, chan_name);
			strcpy(head->channel, chan_name);
			print_in_channel("You have been placed in a private channel\n", chan_name);
			clear_str(target_name);
			return ;
		}
		head = head->next;
	}
	output_error(client->fd, "User not found\n");
	clear_str(target_name);
}
