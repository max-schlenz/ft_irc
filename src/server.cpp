# include "Server.hpp"

void	exiting(int error_code);

void Server::accept_client()
{
	Client client = Client();
	client.set_id(accept(this->_sock, (struct sockaddr*)&this->_sin, &this->_sinLen));
	if (client.id() < 0) {
		exiting(3);
	}
	this->_clients.push_back(client);
}