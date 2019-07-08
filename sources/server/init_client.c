#include "irc_server.h"

int				is_name_taken(char *name)
{
	t_client	*head = get_client(NULL);
	while (head)
	{
		if (strcmp(head->name, name) == 0)
			return (1);
		head = head->next;
	}
	return (0);
}

t_defined_users	*is_in_passwords(char *name)
{
	int		i = 0;

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
		output_command(fd, "Password required : ");
		if (strcmp(get_input(fd), password->password) != 0)
		{
			output_error(fd, "Wrong password\n");
			close_client(fd);
		}
		char *tmp;
		tmp = ft_strjoin("*", name);
		strcpy(name, tmp);
		clear_str(tmp);
	}
	return (name);
}

char		*read_name(int fd)
{
	char			*name;

	output_command(fd, "Please choose a name : ");
	while ((name = get_input(fd)))
	{
		if (strlen(name) > 31)
			output_error(fd, "31 char max !\n");
		else if (strchr(name, '*') != NULL || strchr(name, '%') != NULL || strchr(name, ' ') != NULL)
			output_error(fd, "Forbidden char [*% ]\n");
		else if (is_name_taken(name) == 1)
			output_error(fd, "Name already used\n");
		else
			break;
		output_command(fd, "Please choose a name : ");
	}
	name = check_user(name, fd);
	return (name);
}

t_client	*init_client(int fd)
{
	t_client	*client;
	char		*tmp;

	PROT(client = malloc(sizeof(t_client)), 0, "malloc");
	bzero(client, sizeof(t_client));
	output_command(fd, "Welcome to HAF irc server :)\n");
	tmp = read_name(fd);
	strcpy(client->name, tmp);
	clear_str(tmp);
	output_command(fd, "Successfully logged !\n");
	output_command(fd, "Type /help to have a list of commands\n");
	client->fd = fd;
	if (strchr(client->name, '*') != NULL)
		strcpy(client->channel, "General");
	else
		strcpy(client->channel, "La poubelle");
	return (client);
}
