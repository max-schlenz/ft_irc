#include <iostream>
#include "server.h"
#include <vector>

static void	exiting(int error_code)
{
	if (error_code == 0)
		std::cout << "Invalid Arguments" << std::endl;
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
	int client;
	t_server	server;
	int bufSize = 1024;
	char buffer_arr[bufSize]; // = {'\0'};
	std::string buffer("test");
	socklen_t size;

	if (argc != 3)
		exiting(0);
	server.port = atoi(argv[1]);
	init_server(&server);
	client = accept(server.sock, (struct sockaddr*)&server.sin, &size);
	
	if (client < 0)
		exiting(3);
	buffer = "Server connected\n";
	send(client, buffer.c_str(), buffer.size(), 0);
	buffer = "Please Authenticate\n";
	send(client, buffer.c_str(), buffer.size(), 0);
	while (client > 0)
	{
		buffer = "Username\n> ";
		// better_strcpy(buffer, "Username\n> ", bufSize);
		send(client, buffer.c_str(), buffer.size(), 0);
		do {
			// buffer = "\0";
			memset(buffer_arr, 0, bufSize);
			recv(client, &buffer_arr, bufSize, 0);

			std::cout << buffer_arr;

		} while (buffer_arr[0] != '#');
			std::cout << std::endl;
		close(server.sock);
		close(client);
		exit(0);
	}
	return (0);
}