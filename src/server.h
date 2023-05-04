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

typedef struct s_server t_server;

struct s_server
{
	int	sock;
	int port;
	struct	sockaddr_in sin;
};

# endif