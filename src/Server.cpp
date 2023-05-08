# include "Server.hpp"

void	exiting(int error_code);

void Server::accept_client()
{
	Client client = Client();
	socklen_t size = sizeof(client.saddr_in());
	// client.set_id(accept(this->_sock, (struct sockaddr*)&this->_saddr_in, &size));
	client.set_id(accept(this->_sock, (struct sockaddr*)&client.saddr_in(), &size));
	if (client.id() < 0) {
		exiting(3);
	}
	client.set_sinLen(sizeof(this->_saddr_in));
	this->_clients.push_back(client);
}