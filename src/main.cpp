#include <iostream>
#include <vector>
#include <stdlib.h> //atoi linux
#include "Server.hpp"
#include "Client.hpp"
#include "irc.hpp"

int main(int argc, char **argv)
{
	int		bufSize = 1024;
	char	buffer_arr[bufSize];
	struct sockaddr_in	client_ip;
	socklen_t 			client_ip_len = sizeof(client_ip);
	std::string 		client_ip_str;

	Client	client;
	Server	server;

	int			recv_len = 0;

	if (argc != 2)
		exiting(0);

	init_server(server, atoi(argv[1]));

	std::cout << "Server listening on: " << BWHITE << inet_ntoa(server.sin().sin_addr) << ":" << server.port() << RESET <<  std::endl;

	while (true) // outer loop which waits for connection
	{

		// server.accept_client();
		// client = server.client();

		// getsockname(client.id(), (struct sockaddr*)&client.sin(), &client.sinLen());
		// client.set_ipstr(inet_ntoa(client.sin().sin_addr));

		// std::cout << GREEN << "Client " << BGREEN << client.ipStr() << GREEN << " connected." << RESET << std::endl;
		server.accept_client();
		client = server.client();
		// if (client < 0)
		// 	exiting(3);

		// set_sin()
		getsockname(client.id(), (struct sockaddr*)&(client.sin()), &(client.sinLen()));
		client.set_ipstr(inet_ntoa(client.sin().sin_addr));
		std::cout << GREEN << "Client " << BGREEN << client.ipStr() << GREEN << " connected." << RESET << std::endl;

		while (true) // inner loop for recieving messages from currently connected client
		{
			memset(buffer_arr, 0, bufSize);
			recv_len = recv(client.id(), &buffer_arr, bufSize, 0);
			if (recv_len <= 0 || (buffer_arr[0] == '#'))
			{
				std::cout << RED << "Client " << BRED << client.ipStr() << RED << " disconnected." << RESET << std::flush;
				close(client.id());
				break;
			}
			std::cout << buffer_arr;
		}
		std::cout << std::endl;
	}
}
