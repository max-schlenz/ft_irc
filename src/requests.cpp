# include "Server.hpp"

void Server::handleReqHandshake(Client& client, std::vector<std::string> reqVec)
{
	std::string response = ":127.0.0.1 001 mschlenz :\002\037\0038Welcome to our dmt.irc server!\002\037\0038\r\n";
	send(client.getSock(), response.c_str(), response.size(), 0);
	response = "\033[0;34m[CAP LS]\033[0m no available capabilities\r\n";
	send(client.getSock(), response.c_str(), response.size(), 0);
}

void Server::handleReqPing(Client& client, std::vector<std::string> reqVec)
{
	if (reqVec.size() > 1)
	{
		std::string response = "PONG " + reqVec[1] + "\r\n";
		send(client.getSock(), response.c_str(), response.size(), 0);
		std::cout << GRAY << "PING recieved from " << client.getIpStr() << " (" << client.getNickname() << ")" << std::endl;
	}
}

//MUST NOT contain any of the following characters: space (' ', 0x20), comma (',', 0x2C), asterisk ('*', 0x2A), question mark ('?', 0x3F), exclamation mark ('!', 0x21), at sign ('@', 0x40)
//MUST NOT start with any of the following characters: dollar ('$', 0x24), colon (':', 0x3A)
//MUST NOT start with a character listed as a channel type prefix
//SHOULD NOT contain any dot character ('.', 0x2E)
void Server::handleReqNick(Client& client, std::vector<std::string> reqVec)
{
	if (reqVec.size() > 1)
	{
		client.setNickname(reqVec[1]);
		std::cout << GRAY << "NICK set to " << client.getNickname() << std::endl;
	}
}

void Server::handleReqUser(Client& client, std::vector<std::string> reqVec)
{	
	if (reqVec.size() > 1)
	{
		client.setUsername(reqVec[1]);
		std::cout << GRAY << "USER set to " << client.getUsername() << std::endl;
	}
}

void Server::handleReqMode(Client& client, std::vector<std::string> reqVec)
{
	std::cout << GRAY << "MODE received (unhandled)" << RESET << std::endl;
}

void Server::handleReqQuit(Client& client, int i)
{
	std::cout << RED << "Client " << BRED << client.getIpStr() << RED << " disconnected." << RESET << std::endl;
	close(this->_pollFds[i].fd);
	this->_pollFds.erase(this->_pollFds.begin() + i);
	this->_clients.erase(this->_clients.begin() + (i - 1));
}