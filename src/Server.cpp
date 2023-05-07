# include "Server.hpp"

void	exiting(int error_code);

void Server::accept_client()
{
	Client client = Client();
	socklen_t size = sizeof(this->_sin);
	client.set_id(accept(this->_sock, (struct sockaddr*)&this->_sin, &size));
	if (client.id() < 0) {
		exiting(3);
	}
	client.set_sinLen(sizeof(this->_sin));
	this->_clients.push_back(client);
}