#include "irc_server.h"

void			display_help(int fd)
{
	dprintf(fd, "/chan info : display chan name and connected users\n");
	dprintf(fd, "/chan set [name] : change your channel\n");
	dprintf(fd, "/private [user_name] : creates a private channel with the user\n");
	dprintf(fd, "/msg [user_name] : ask you to type a message and send it to the user\n");
	dprintf(fd, "/help : display this\n");
	dprintf(fd, "/quit : exit the server\n");
}

void		kick_user(int fd, char *user_name)
{
	t_client	*head;

	if (strlen(user_name) > 31)
		return ;
	head = get_client(NULL);
	while (head)
	{
		if (strcmp(head->name, user_name) == 0)
		{
			output(head->fd, "You've been kicked, don't play shit with me bro\n");
			close_client(head->fd);
			return ;
		}
		head = head->next;
	}
	output_error(fd, "User not found\n");
}

void		execute_action(char *input, t_client *client, int fd)
{
	if (strncmp(input, "/chan set ", 10) == 0)
		change_channel(client, strdup(&input[10]), fd);
	else if (strncmp(input, "/private ", 9) == 0)
		private_channel(client, strdup(&input[9]));
	else if (strncmp(input, "/msg ", 5) == 0)
		send_msg(client, strdup(&input[5]));
	else if (strcmp(input, "/chan info") == 0)
		display_channel(client, fd);
	else if (strcmp(input, "/quit") == 0)
		close_client(fd);
	else if (strncmp(input, "/kick ", 6) == 0 && strcmp(client->name, "*admin") == 0)
		kick_user(fd, &input[6]);
	else if (strcmp(input, "/help") == 0)
		display_help(fd);
	else if (input[0] != '/')
		msg_in_channel(input, client);
	else
		output_error(client->fd, "Unrecognised command\n");
}

void	*receive_data(void *arg)
{
	int			client_fd;
	char		*input;
	t_client	*client;

	client_fd = (int)arg;
	client = init_client(client_fd);
	add_client(client);
	while (1)
	{
		input = get_input(client_fd);
		execute_action(input, client, client_fd);
		clear_str(input);
	}
}