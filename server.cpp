#include <iostream>
#include "server.h"
#include <vector>
#include <stdlib.h> //atoi linux

static void	exiting(int error_code)
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


static void init_server(t_server *server)
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

int main(int argc, char **argv)
{
	int		bufSize = 1024;
	char	buffer_arr[bufSize];

	struct sockaddr_in	client_ip;
	socklen_t 			client_ip_len = sizeof(client_ip);
	std::string 		client_ip_str;

	t_server	server;
	socklen_t	size;

	int			recv_len = 0;

	if (argc != 2)
		exiting(0);

	server.port = atoi(argv[1]);
	init_server(&server);

	std::cout << "Server listening on: " << BWHITE << inet_ntoa(server.sin.sin_addr) << ":" << server.port << RESET <<  std::endl;
	while (true)
	{
		int client = accept(server.sock, (struct sockaddr*)&server.sin, &size);
		if (client < 0)
			exiting(3);

		getsockname(client, (struct sockaddr*)&client_ip, &client_ip_len);
		client_ip_str = inet_ntoa(client_ip.sin_addr);
		std::cout << GREEN << "Client " << BGREEN << client_ip_str << GREEN << " connected." << RESET << std::endl;

		while (true) // 
		{
			memset(buffer_arr, 0, bufSize);
			recv_len = recv(client, &buffer_arr, bufSize, 0);
			if (recv_len <= 0 || (buffer_arr[0] == '#'))
			{
				std::cout << RED << "Client " << BRED << client_ip_str << RED << " disconnected." << RESET << std::flush;
				close(client);
				break;
			}
			std::cout << buffer_arr;
		}
		std::cout << std::endl;
	}
}