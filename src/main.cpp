#include <iostream>
#include <vector>
#include <stdlib.h> //atoi linux
#include "Server.hpp"
#include "Client.hpp"
#include "irc.hpp"
#include <signal.h>


void sigint_handler(int sig) 
{
  std::cout << "Exiting programm..." << std::endl;
  exit(EXIT_SUCCESS);
}

void sig()
{
	if (signal(SIGINT, sigint_handler) == SIG_ERR) 
	{
		std::cout << "Error: Unable to register signal handler!" << std::endl;
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char **argv)
{
	if (argc != 2)
		exiting(0);
	sig();

	Server server(atoi(argv[1]));
	server.startServer();

	return (EXIT_SUCCESS);
}
