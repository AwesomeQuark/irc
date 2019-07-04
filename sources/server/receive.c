#include "irc_server.h"

t_defined_users	*is_in_passwords(char *name)
{
	int		i;

	i = 0;
	while (passwords[i].name != NULL)
	{
		if (strcmp(passwords[i].name, name) == 0)
			return (&passwords[i]);
		i++;
	}
	return (NULL);
}

char		*check_user(char *name, int fd)
{
	t_defined_users	*password;

	if ((password = is_in_passwords(name)) != NULL)
	{
		output(fd, "Password required : ");
		if (strcmp(get_input(fd), password->password) != 0)
		{
			output(fd, "Wrong password\n");
			close_client(fd);
		}
		char *tmp;
		tmp = ft_strjoin("\033[32m*\033[0m", name);
		free(name);
		name = tmp;
	}
	return (name);
}

char		*read_name(int fd)
{
	char			*name;

	output(fd, "Please choose a name : ");
	while ((name = get_input(fd)))
	{
		if (strlen(name) > 32)
			output(fd, "32 char max !\nPlease choose a name : ");
		else if (strchr(name, '*') != NULL || strchr(name, '%') != NULL)
			output(fd, "Forbidden char\nPlease choose a name : ");
		else
			break;
	}
	name = check_user(name, fd);
	return (name);
}

t_client	*init(int fd)
{
	t_client	*client;

	PROT(client = malloc(sizeof(t_client)), 0, "malloc");
	bzero(client, sizeof(t_client));
	output(fd, "Welcome to HAF irc server :)\n");
	client->name = read_name(fd);
	client->fd = fd;
	if (strchr(client->name, '*') != NULL)
		client->channel = strdup("General");
	else
		client->channel = strdup("La poubelle");
	return (client);
}

void		execute_action(char *input, t_client *client, int fd)
{
	if (strncmp(input, "/chan set ", 10) == 0)
		change_channel(client, strdup(&input[10]), fd);
	else if (strcmp(input, "/chan info") == 0)
		display_channel(client, fd);
	//else if (strcmp(input, "/help") == 0)
	//	display_help(fd);
	else
		msg_in_channel(input, client->channel, client->name);
}

void	*receive_data(void *arg)
{
	int			client_fd;
	char		*input;
	t_client	*client;

	client_fd = (int)arg;
	client = init(client_fd);
	add_client(client);
	while (1)
	{
		input = get_input(client_fd);
		execute_action(input, client, client_fd);
		free(input);
	}
}