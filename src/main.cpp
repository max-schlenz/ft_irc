#include <iostream>
#include <vector>
#include <stdlib.h> //atoi linux
#include "Server.hpp"
#include "irc.hpp"

int main(int argc, char **argv)
{
	int		bufSize = 1024;
	char	buffer_arr[bufSize];

	struct sockaddr_in	client_ip;
	socklen_t 			client_ip_len = sizeof(client_ip);
	std::string 		client_ip_str;

	Server	server;
	// socklen_t	size;

	int			recv_len = 0;

	if (argc != 2)
		exiting(0);

	// server.port = atoi(argv[1]);
	init_server(server, atoi(argv[1]));

	std::cout << "Server listening on: " << BWHITE << inet_ntoa(server.sin().sin_addr) << ":" << server.port() << RESET <<  std::endl;

	while (true) // outer loop which waits for connection
	{
		// int client = accept(server.sock, (struct sockaddr*)&server.sin, &size);
		int client = server.accept_client();
		// if (client < 0)
		// 	exiting(3);

		getsockname(client, (struct sockaddr*)&client_ip, &client_ip_len);
		client_ip_str = inet_ntoa(client_ip.sin_addr);
		std::cout << GREEN << "Client " << BGREEN << client_ip_str << GREEN << " connected." << RESET << std::endl;

		while (true) // inner loop for recieving messages from currently connected client
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
