#include <iostream>
#include "server.h"
#include <vector>
#include <stdlib.h> //atoi linux

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

	std::vector<std::string> recieved_data;

	std::string command;

	int client = 0;

	while (true) // outer loop which waits for connection
	{
		client = accept(server.sock, (struct sockaddr*)&server.sin, &size);
		if (client < 0)
			exiting(3);

		getsockname(client, (struct sockaddr*)&client_ip, &client_ip_len);
		client_ip_str = inet_ntoa(client_ip.sin_addr);
		std::cout << GREEN << "Client " << BGREEN << client_ip_str << GREEN << " connected." << RESET << std::endl;

		do {
			memset(buffer_arr, 0, bufSize);
			recv_len = recv(client, &buffer_arr, bufSize, 0);
			if (buffer_arr[0] == '#')
				break ;
			std::string buffer_str = buffer_arr;
			command += buffer_str;
			size_t command_nl_pos = buffer_str.find('\n');
			if (command.length() && command_nl_pos == buffer_str.npos)
				command += '\n';	// placeholder to seperate commands, maybe use vector?
			if (!command.length() || command_nl_pos != buffer_str.npos)
			{
				std::cout << command << std::flush;	// placeholder to execute commands one by one
				command.clear();
			}
		} while (recv_len > 0);
		
		std::cout << RED << "Client " << BRED << client_ip_str << RED << " disconnected." << RESET << std::endl;
		close(client);
	}

}
