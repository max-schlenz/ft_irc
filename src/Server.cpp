# include "Server.hpp"

void	exiting(int error_code);

void Server::accept_client()
{
	Client client = Client();
	socklen_t size = sizeof(this->_sin);
	int id = accept(this->_sock, (struct sockaddr*)&this->_sin, &size);
	client.set_id(id);
	if (client.id() > 0) {
		// exiting(3);
		client.set_sinLen(sizeof(this->_sin));
		getsockname(client.id(), (struct sockaddr*)&client.sin(), &client.sinLen());
		client.set_ipstr();
		std::cout << GREEN << "Client " << BGREEN << client.ipStr() << GREEN << " connected." << RESET << std::endl;
		this->_clients.push_back(client);
	}
}
