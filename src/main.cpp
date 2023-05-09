#include <iostream>
#include <vector>
#include <stdlib.h> //atoi linux
#include "Server.hpp"
#include "Client.hpp"
#include "irc.hpp"
#include <signal.h>
#include <poll.h>

void sigint_handler(int sig) {
  std::cout << "Exiting programm..." << std::endl;
  exit(EXIT_SUCCESS);
}

int main(int argc, char **argv)
{
	int		bufSize = 1024;
	char	buffer_arr[bufSize];

	Client	client;

	int			recv_len = 0;

	if (argc != 2)
		exiting(0);

	Server	server(atoi(argv[1]));
	std::cout << "Server listening on: " << BWHITE << inet_ntoa(server.sin().sin_addr) << ":" << server.port() << RESET <<  std::endl;
	if (signal(SIGINT, sigint_handler) == SIG_ERR) {
		std::cout << "Error: Unable to register signal handler!" << std::endl;
		exit(EXIT_FAILURE);
	}
	std::vector<pollfd> poll_fds;
	std::vector<std::string> cmd_queue;

    pollfd server_poll_fd;
	memset(&server_poll_fd, 0, sizeof(server_poll_fd));
    server_poll_fd.fd = server.sock();
    server_poll_fd.events = POLLIN;
    poll_fds.push_back(server_poll_fd);
	memset(buffer_arr, 0, bufSize);
	while (1) // outer loop which waits for connection
	{
		int res = poll(poll_fds.data(), poll_fds.size(), 1000);
		if (res == -1)
			exiting(5);
		else if (res > 0)
		{
			for (int i = 0; i < poll_fds.size(); i++) {
				if (poll_fds[i].fd == server.sock() && poll_fds[i].revents & POLLIN) {
					server.accept_client();
					client = server.client();
					pollfd client_poll_fd;
					client_poll_fd.fd = client.id();
					client_poll_fd.events = POLLIN;
					poll_fds.push_back(client_poll_fd);
				} else if (poll_fds[i].revents & POLLIN) {
						recv_len = recv(poll_fds[i].fd, &buffer_arr, bufSize, 0);
						if (recv_len <= 0)
						{
							std::cout << RED << "Client " << BRED << client.ipStr() << RED << " disconnected." << RESET << std::endl;
							close(poll_fds[i].fd);
							poll_fds.erase(poll_fds.begin() + i);
							memset(buffer_arr, 0, bufSize);
						}
						else
						{
							cmd_queue.push_back(buffer_arr);
							if (strchr(buffer_arr, '\n'))
							{
								for (std::vector<std::string>::iterator it = cmd_queue.begin(); it != cmd_queue.end(); ++it)
									std::cout << *it << std::flush;
								cmd_queue.clear();
							}
							memset(buffer_arr, 0, bufSize);
						}
				}
			}
		}
	}
	return (EXIT_SUCCESS);
}
