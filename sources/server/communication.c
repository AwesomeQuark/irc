#include "irc_server.h"

char	*get_input(int fd)
{
	char	*input;
	char	buff[64];

	PROT(input = strdup(""), 0, "ft_strdup");
	while (strchr(input, '\n') == NULL)
	{
		ft_bzero(buff, 64);
		if (read(fd, buff, 63) <= 0)
			close_client(fd);
		PROT(input = ft_strjoin(input, buff), 0, "strjoin")
	}
	input[ft_strlen(input) - 1] = 0;
	return (input);
}

int		output(int fd, char *msg)
{
	send(fd, msg, ft_strlen(msg), 0);
}