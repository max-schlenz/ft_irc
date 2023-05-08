#include <iostream>
#include <vector>
#include <stdlib.h> //atoi linux
#include "Server.hpp"
#include "Client.hpp"
#include "irc.hpp"
#include <signal.h>
#include <poll.h>

void sigint_handler(int sig) {
  printf("Exiting programm...\n");
  exit(EXIT_SUCCESS);
}

int main(int argc, char **argv)
{
	int		bufSize = 1024;
	char	buffer_arr[bufSize];

	Client	client;
	Server	server;

	int			recv_len = 0;

	if (argc != 2)
		exiting(0);

	init_server(server, atoi(argv[1]));
	std::cout << "Server listening on: " << BWHITE << inet_ntoa(server.sin().sin_addr) << ":" << server.port() << RESET <<  std::endl;
	if (signal(SIGINT, sigint_handler) == SIG_ERR) {
		printf("Error: Unable to register signal handler!\n");
		exit(EXIT_FAILURE);
	}
	std::vector<pollfd> poll_fds;

    pollfd server_poll_fd;
	memset(&server_poll_fd, 0, sizeof(server_poll_fd));
    server_poll_fd.fd = server.sock();
    server_poll_fd.events = POLLIN;
    poll_fds.push_back(server_poll_fd);
	while (true) // outer loop which waits for connection
	{
		int res = poll(poll_fds.data(), poll_fds.size(), 5000);
		std::cout << "debug: poll" << std::endl;
		if (res == -1)
			exiting(5);
		else if (res > 0)
		{
			server.accept_client();
			client = server.client();
			// getsockname(client.id(), (struct sockaddr*)&client.sin(), &client.sinLen());
			// client.set_ipstr();
			// std::cout << GREEN << "Client " << BGREEN << client.ipStr() << GREEN << " connected." << RESET << std::endl;
			// while (true) // inner loop for recieving messages from currently connected client
			// {
			memset(buffer_arr, 0, bufSize);
			recv_len = recv(client.id(), &buffer_arr, bufSize, 0);
			if (buffer_arr[0] == '#')
			{
				std::cout << RED << "Client " << BRED << client.ipStr() << RED << " disconnected." << RESET << std::flush;
				close(client.id());
				// break;
			}
				std::cout << buffer_arr << std::endl;
			// }
			// std::cout ;
		}
	}
	return (EXIT_SUCCESS);
}
