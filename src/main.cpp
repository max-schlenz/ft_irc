#include <iostream>
#include <vector>
#include <cstdlib>
#include "Server.hpp"
#include "Client.hpp"
#include "irc.hpp"
#include <signal.h>
#include "definitions.hpp"

bool g_run = true;

void sigint_handler(int sig) 
{
	std::cerr << std::endl << "Exiting programm..." << std::endl;
	g_run = false;
}

void sig()
{
	if (signal(SIGINT, sigint_handler) == SIG_ERR) 
	{
		std::cerr << std::endl << "Error: Unable to register signal handler!" << std::endl;
		g_run = false;
	}
}

int main(int argc, char **argv)
{
	int port = 6667;

	if (argc == 2)
		port = std::atoi(argv[1]);

	sig();

	Server server(port);
	server.startServer();

	return (0);
}

