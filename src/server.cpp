# include "Server.hpp"

void	exiting(int error_code);

int Server::accept_client()
{
	socklen_t size;
	int client;

	client = accept(this->_sock, (struct sockaddr*)&this->_sin, &size);
	if (client < 0)
		exiting(3);
	return (client);
}
