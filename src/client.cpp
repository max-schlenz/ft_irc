#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <netinet/in.h>

int main()
{
	int client, server;
	int portNum = 5400;
	int bufSize = 1024;
	// char *ip = "127.0.0.1";
	char buffer[bufSize];
	bool isExit = false;
	socklen_t size;

	client = socket(AF_INET, SOCK_STREAM, 0);
	if (client < 0) {
		std::cerr << "Error establishing connection..." << std::endl;
		return (1);
	}
	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(4242);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	// inet_pton(AF_INET, "0.0.0.0", &server_addr.sin_addr); //

	if (connect(client, (struct sockaddr*)&server_addr, sizeof(server_addr)) == 0)
	{
		std::cout << "Connection to server" << std::endl;
	}
	recv(client, buffer, bufSize, 0);
	std::cout << "Connection confirmed" << std::endl;
	std::cout << "Enter # to end the connection" << std::endl;
	do {
			std::cout << "Client: ";
			do {
				std::cin >> buffer;
				send(client, buffer, bufSize, 0);
				if (*buffer == '#')
				{
					send(client, buffer, bufSize, 0);
					*buffer = '*';
					isExit = true;
				}
			} while (*buffer != '*');
			std::cout << "Server: ";
			do {
				recv(client, buffer, bufSize, 0);
				std::cout << "buffer" << " ";
				if (*buffer == '#')
				{
					*buffer = '*';
					isExit = true;
				}
			} while (*buffer != '*');
	} while (!isExit);
	std::cout << "Connection terminated..." << std::endl;
	return (0);
}