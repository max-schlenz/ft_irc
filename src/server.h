#ifndef SERVER_H
# define SERVER_H
# include <sys/socket.h>
# include <sys/types.h>
# include <netdb.h>
# include <unistd.h>
# include <arpa/inet.h>
# include <string.h>
# include <string>
# include <netinet/in.h>
# include <cstdlib>
# include <iostream>

# define RED "\033[0;31m"
# define BRED "\033[1;31m"
# define GREEN "\033[0;32m"
# define BGREEN "\033[1;32m"
# define BLUE "\033[0;34m"
# define BBLUE "\033[1;34m"
# define GRAY "\033[0m\033[38;5;239m"
# define BWHITE "\033[1;37m"
# define RESET "\033[0m"

typedef struct s_server t_server;

struct s_server
{
	int	sock;
	int port;
	struct	sockaddr_in sin;
};

void init_server(t_server *server);
void exiting(int error_code);

# endif