# include "Server.hpp"

void Server::acceptClient()
{
	sockaddr_in sin;
	socklen_t size = sizeof(sin);
	int sock = accept(this->_sock, (struct sockaddr*)&sin, &size);
	if (sock > 0)
	{
		getsockname(sock, (struct sockaddr*)&sin, &size);
		std::string ipStr = inet_ntoa(sin.sin_addr);

		pollfd client_poll_fd;
		client_poll_fd.fd = sock;
		client_poll_fd.events = POLLIN;
		this->_pollFds.push_back(client_poll_fd);

		Client client(sin, sock, ipStr, client_poll_fd);
		this->_clients.push_back(client);
	}
}

void Server::disconnectClient(Client& client, int i)
{
	this->partAllChannels(client);
	close(client.getSock());
	close(this->_pollFds[i].fd);
	std::cout << RED << "Client " << BRED << this->_clients[i - 1].getHostname() << RED << " disconnected." << RESET << std::endl;
	this->_clientsM.erase(this->_clients[i - 1].getNickname());
	this->_pollFds.erase(this->_pollFds.begin() + i);
	this->_clients.erase(this->_clients.begin() + (i - 1));
}
