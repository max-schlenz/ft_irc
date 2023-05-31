# include "Server.hpp"

extern bool g_run;

void Server::setSock(int sock) {
	this->_sock = sock;
};
void Server::setPort(int port) {
	this->_port = port;
};
void Server::setKey(std::string key) {
	this->_key_set = true;
	this->_key = key;
};
void Server::setSin(sockaddr_in _saddr_in) {
	this->_saddr_in_len = sizeof(_saddr_in);
	this->_saddr_in = _saddr_in;
};
void Server::setSinLen(socklen_t _saddr_in_len) {
	this->_saddr_in_len = _saddr_in_len;
}

const int& Server::getSock() const {
	return (this->_sock);
}
const int& Server::getPort() const {
	return (this->_port);
}
const std::string& Server::getHostname() const {
	return (this->_hostname);
}
const std::string& Server::getKey() const {
	return (this->_key);
}
const bool& Server::getKeySet() const {
	return (this->_key_set);
}
sockaddr_in& Server::getSin() {
	return (this->_saddr_in);
}
Client& Server::getClient(int i) {
	return (this->_clients[i]);
}
std::vector<Client>& Server::getClients() {
	return this->_clients;
}
std::string& Server::getDateString() {
	return this->_dateString;
}
socklen_t& Server::getSinLen() {
	return (this->_saddr_in_len);
}
std::map<std::string, Client*>& Server::getClientsM() {
	return this->_clientsM;
}
std::map<std::string, Channel>& Server::getChannelsM() {
	return this->_channelsM;
}

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
	if (client.getConnected())
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

Client &Server::getClientName(std::string name)
{
	std::vector<Client>::iterator it = this->_clients.begin();
	for (; it != this->_clients.end(); ++it)
	{
		if (it->getNickname() == name)
			return (*it);
	}
	return *it;
}

void Server::startServer()
{
	int res = 0;

	if (g_run)
		std::cout << "~~~ Server listening on: " << BWHITE << inet_ntoa(this->_saddr_in.sin_addr) << ":" << this->_port << " ~~~\n" << RESET <<  std::endl;
	while (g_run)
	{
		res = poll(this->_pollFds.data(), this->_pollFds.size(), 500);
		for (unsigned int i = 0; res > 0 && i < this->_pollFds.size(); i++)
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
	if (this->_bot_on)
		delete this->_bot;
}