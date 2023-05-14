# include "Server.hpp"

void Server::handleReqHandshake(Client& client, std::vector<std::string> reqVec)
{
	std::string response = ":127.0.0.1 001 mschlenz :Welcome to the Internet Relay Network mschlenz!mschlenz@mschlenz\r\n";
	send(client.sock(), response.c_str(), response.size(), 0);
}

void Server::handleReqPing(Client& client, std::vector<std::string> reqVec)
{
	if (reqVec.size() > 1)
	{
		std::string response = "PONG " + reqVec[1] + "\r\n";
		send(client.sock(), response.c_str(), response.size(), 0);
	}
}

void Server::handleReqNick(Client& client, std::vector<std::string> reqVec)
{
	client.setNickname(reqVec[1]);
	std::cout << GRAY << "NICK set to " << client.getNickname() << std::endl;
}

void Server::handleReqUser(Client& client, std::vector<std::string> reqVec)
{	
	client.setUsername(reqVec[1]);
	std::cout << GRAY << "USER set to " << client.getUsername() << std::endl;
}

void Server::handleReqMode(Client& client, std::vector<std::string> reqVec)
{
	std::cout << GRAY << "MODE received (unhandled)" << RESET << std::endl;
}

void Server::handleReqQuit(Client& client, int i)
{
	std::cout << RED << "Client " << BRED << client.ipStr() << RED << " disconnected." << RESET << std::endl;
	close(this->_pollFds[i].fd);
	this->_pollFds.erase(this->_pollFds.begin() + i);
	this->_clients.erase(this->_clients.begin() + (i - 1));
}