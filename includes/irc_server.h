#ifndef IRC_SERVER_H
# define IRC_SERVER_H

# include <netdb.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <printf.h>
# include <arpa/inet.h>
# include <stdlib.h>
# include "../haflib/includes/haflib.h"

# define SUCCESS 1
# define FAILURE 0

// SYS FUNCTIONS PROTECTION
void	server_quit(int status);
# define PROT(function, err_no, msg) if ((int)(function) == err_no) {perror(msg); server_quit(FAILURE);}

# define MAX_CONNECTIONS 50
# define USAGE_SERVER printf("Usage: ./%s port", av[0])
# define USAGE_CLIENT printf("Usage: ./%s hostname port", av[0])

//########################
//###  FUNCTIONS SERV  ###
//########################

//	INITIALISATION socket
int		server_init_socket(int port);
void	fill_capacity(struct sockaddr_in *capa, int port);


//	socket FD SINGLETONE
int		get_socket_fd(int fd);
int		set_sokcet_fd(int fd);

//	MAIN LOOP
void	server_listen(int fd);
void	init_fd_set(fd_set *set, int socket_fd);
void	new_connection(int *client_fd, int socket_fd, fd_set *set);
void	receive_data(int *client_fd, fd_set *set);
void	close_client(int *client_fd, int i);

#endif
