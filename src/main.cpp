#include <iostream>
#include <vector>
#include <stdlib.h> //atoi linux
#include "Server.hpp"
#include "Client.hpp"
#include "irc.hpp"
#include <signal.h>

void handleClientReq(std::vector<pollfd>& poll_fds, Server& server, int i);

void sigint_handler(int sig) {
  std::cout << "Exiting programm..." << std::endl;
  exit(EXIT_SUCCESS);
}

void sig()
{
	if (signal(SIGINT, sigint_handler) == SIG_ERR) {
		std::cout << "Error: Unable to register signal handler!" << std::endl;
		exit(EXIT_FAILURE);
	}
}

void handleClientReq(std::vector<pollfd>& poll_fds, Server& server, int i)
{
	int		bufSize = 1024;
	char	buffer_arr[bufSize];
	Client	client;
	int		recv_len = 0;
	std::vector<std::string> cmd_queue;

	memset(buffer_arr, 0, bufSize);
	recv_len = recv(poll_fds[i].fd, &buffer_arr, bufSize, 0);
	if (recv_len <= 0)
	{
		client = server.client(i - 1);
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

int main(int argc, char **argv)
{
	if (argc != 2)
		exiting(0);

	Server	server(atoi(argv[1]));

	server.startServer();

	return (EXIT_SUCCESS);
}
