# include "Server.hpp"

extern bool g_run;

bool Server::isUserInChannel(Client &client, std::string channelName)
{
	if (client.getJoinedChannels().find(channelName) != client.getJoinedChannels().end())
		return true;
	return false;
}

void Server::sendMsgToAll(Client &client, std::string message)
{
	for (std::map<std::string, Client*>::iterator it = this->_clientsM.begin(); it != this->_clientsM.end(); ++it)
	{
		if (it->first != client.getNickname())
			send(it->second->getSock(), message.c_str(), message.size(), 0);
	}
}

void Server::sendResponse(Client &client, const std::string& response)
{
	std::cout << PINK << response << RESET << std::flush;
	send(client.getSock(), response.c_str(), response.size(), 0);
}

bool Server::isValidClient(std::string name)
{
	for (std::vector<Client>::iterator it = this->_clients.begin(); it != this->_clients.end(); ++it)
	{
		if (it->getNickname() == name)
			return true;
	}
	return false;
}

// ! use isValidClient() as protection !
Client &Server::getClientName(std::string name)
{
	for (std::vector<Client>::iterator it = this->_clients.begin(); it != this->_clients.end(); ++it)
	{
		if (it->getNickname() == name)
			return (*it);
	}
	Client *err;
	return *(err);
}

void Server::startServer()
{
	int res = 0;

	if (g_run)
		std::cout << "~~~ Server listening on: " << BWHITE << inet_ntoa(this->_saddr_in.sin_addr) << ":" << this->_port << " ~~~\n" << RESET <<  std::endl;
	while (g_run)
	{
		res = poll(this->_pollFds.data(), this->_pollFds.size(), 1000);
		for (int i = 0; res > 0 && i < this->_pollFds.size(); i++)
		{
			if (this->_pollFds[i].fd == this->_sock && this->_pollFds[i].revents & POLLIN && this->_clients.size() < USR_LIMIT)
				this->acceptClient();

			else if (i > 0 && this->_pollFds[i].revents & POLLIN)
			{
				if (!this->handleClientReq(this->_clients[i - 1]) || this->_clients[i - 1].getKick())
					this->disconnectClient(this->_clients[i - 1], i);
			}
		}
	}
}
