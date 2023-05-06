#include "server.h"

void	exiting(int error_code)
{
	if (error_code == 0)
		std::cout << "usage: ./server <port>" << std::endl;
	else if (error_code == 1)
		std::cout << "Error establishing connection..." << std::endl;
	else if (error_code == 2)
		std::cout << "Error binding socket..." << std::endl;
	else if (error_code == 3)
		std::cout << "Error on accepting..." << std::endl;
	else if (error_code == 4)
		std::cout << "Error getting proton.." << std::endl;
	else
		std::cout << "ERROR" << std::endl;
	exit(1);
}


void init_server(t_server *server)
{
	struct	protoent *proto = getprotobyname("tcp");
	struct	sockaddr_in sin;

	if (!proto)
		exiting(4);
	server->sock = socket(AF_INET, SOCK_STREAM, proto->p_proto);
	if (server->sock < 0)
		exiting(1);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(server->port);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(server->sock, (struct sockaddr*)&sin, sizeof(sin)) < 0)
		exiting(2);
	server->sin = sin;
	listen(server->sock, 2);
}
