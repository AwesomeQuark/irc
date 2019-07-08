#ifndef IRC_SERVER_H
# define IRC_SERVER_H

# include <netdb.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <printf.h>
# include <arpa/inet.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <pthread.h>
# include "../haflib/includes/haflib.h"

# define SUCCESS 1
# define FAILURE 0

// SYS FUNCTIONS PROTECTION
void	server_quit(int status);
# define PROT(function, err_no, msg) if ((int)(function) == err_no) {perror(msg); server_quit(FAILURE);}

# define MAX_CONNECTIONS 50
# define USAGE_SERVER printf("Usage: ./%s port", av[0])
# define USAGE_CLIENT printf("Usage: ./%s hostname port", av[0])

typedef	struct	s_client
{
	struct s_client	*next;
	char			name[32];
	char			channel[32];
	int				fd;
}				t_client;

// LIST OF AUTHENTIFICATED USERS
typedef struct	s_defined_users
{
	const char	*name;
	const char	*password;
}				t_defined_users;

extern t_defined_users passwords[];

//########################
//###  FUNCTIONS SERV  ###
//########################

//	INITIALISATION socket
int		server_init_socket(int port);
void	fill_capacity(struct sockaddr_in *capa, int port);

// MAIN loop to check for new connected clients
void	server_listen(int socket_fd);

// one process by loop, always listening to one client
void	*receive_data(void *client_fd);
void	*receive_data_admin(void *none);

// signletones to close socket when ctr-c
int		get_socket_fd(int fd);

// channels and client linked arrays
void	add_client(t_client	*client);
t_client	*get_client(t_client *client);
void	close_client(int client_fd);

// Communication
int		output(int fd, char *msg);
int		output_error(int fd, char *msg);
int		output_command(int fd, char *msg);
void	msg_in_channel(char *msg, t_client *client);
void	print_in_channel(char *msg, char *channel);
char	*get_input(int fd);

// Commands
void	display_channel(t_client *client, int fd);
void	change_channel(t_client *client, char *chan, int fd);
void	display_help(int fd);
void	private_channel(t_client *client, char *target_name);
void	send_msg(t_client *client, char *target);

// client initialisation
char		*read_name(int fd);
t_client	*init_client(int fd);

// signals
void	sig_wrapper(void (*handler)(int no));

//random
void	clear_str(char *str);

#endif
