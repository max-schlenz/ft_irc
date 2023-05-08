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
	memset(buffer_arr, 0, bufSize);
	// int res = 1;
	while (1) // outer loop which waits for connection
	{
		int res = poll(poll_fds.data(), poll_fds.size(), 5000);
		// if (poll_fds.size )
		std::cout << res << std::endl;
		if (res == -1)
			exiting(5);
		// else if (res == 0)
		// {
		// 	std::cout << "res 0" << std::endl;
		// }
		else if (res > 0)
		{
			// std::cout << "Size: " << poll_fds.size() << std::endl;
			for (int i = 0; i < poll_fds.size(); i++) {
				if (poll_fds[i].fd == server.sock() && poll_fds[i].revents & POLLIN) {
					server.accept_client();
					client = server.client();
					pollfd client_poll_fd;
					client_poll_fd.fd = client.id();
					client_poll_fd.events = POLLIN;
					poll_fds.push_back(client_poll_fd);
				} else if (poll_fds[i].revents & POLLIN) {
					// while (true)
					// {
						recv_len = recv(poll_fds[i].fd, &buffer_arr, bufSize, 0);
						if (recv_len <= 0)
						{
							std::cout << RED << "Client " << BRED << client.ipStr() << RED << " disconnected." << RESET << std::flush;
							// close(poll_fds[i].fd);
							poll_fds.erase(poll_fds.begin() + i);
							memset(buffer_arr, 0, bufSize);
							// break;
						}
						else
						{
							std::cout << buffer_arr << std::flush;
							memset(buffer_arr, 0, bufSize);
						}
					// }
				}
			}
			// if (poll_fds.size() > 1) {
			// 	if (poll_fds[1].fd == client.id())
			// 		std::cout << "CLIENT POLLIN" << std::endl;
			// 	std::cout << "test" << std::endl;
			// 	exit(0);
			// }
			
			// getsockname(client.id(), (struct sockaddr*)&client.sin(), &client.sinLen());
			// client.set_ipstr();
			// std::cout << GREEN << "Client " << BGREEN << client.ipStr() << GREEN << " connected." << RESET << std::endl;
			// while (true) // inner loop for recieving messages from currently connected client
			// {
			
			
			// }
			// std::cout ;
		}
	}
	return (EXIT_SUCCESS);
}
