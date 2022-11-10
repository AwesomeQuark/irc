#include "irc_server.h"

void	clear_str(char *str)
{
	bzero(str, strlen(str));
	free(str);
}

void	send_msg(t_client *client, char *target)
{
	t_client	*head = get_client(NULL);;
	char		*msg;

	while (head)
	{
		if (strcmp(head->name, target) == 0)
		{
			output(client->fd, "type your message\n");
			msg = get_input(client->fd);
			dprintf(head->fd, "\033[34m%s >\033[0m %s\n", client->name, msg);
			free(msg);
			return ;
		}
		head = head->next;
	}
	output_error(client->fd, "User not found\n");
}

void	print_in_channel(char *msg, char *channel)
{
	t_client	*head = get_client(NULL);

	while (head)
	{
		if (strcmp(channel, head->channel) == 0)
			dprintf(head->fd, "%s", msg);
		head = head->next;
	}
}

void	msg_in_channel(char *msg, t_client *client)
{
	t_client	*head = get_client(NULL);

	while (head)
	{
		if (strcmp(client->channel, head->channel) == 0)
			dprintf(head->fd, "\033[34m%s >\033[0m %s\n", client->name, msg);
		head = head->next;
	}
}

char	*get_input(int fd)
{
	char	*input;
	char	*tmp;
	char	buff[64];

	PROT(input = strdup(""), 0, "ft_strdup");
	while (strchr(input, '\n') == NULL)
	{
		ft_bzero(buff, 64);
		if (read(fd, buff, 63) <= 0)
			close_client(fd);
		tmp = input;
		PROT(input = ft_strjoin(input, buff), 0, "strjoin")
		ft_bzero(tmp, ft_strlen(tmp));
		free(tmp);
	}
	input[ft_strlen(input) - 1] = 0;
	return (input);
}

int		output(int fd, char *msg)
{
	send(fd, msg, ft_strlen(msg), 0);
}

int		output_error(int fd, char *msg)
{
	send(fd, "\033[31m", 6, 0);
	send(fd, msg, ft_strlen(msg), 0);
	send(fd, "\033[0m", 5, 0);
}

int		output_command(int fd, char *msg)
{
	send(fd, "\033[32m", 6, 0);
	send(fd, msg, ft_strlen(msg), 0);
	send(fd, "\033[0m", 5, 0);
}