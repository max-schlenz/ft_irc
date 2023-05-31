#include "Client.hpp"
#include "Server.hpp"

std::string Client::getRealName()
{
	return this->_realName;
}

Client::Client(sockaddr_in sin, int sock, std::string ipStr, pollfd pollFd)
	: _nickName("default"), _userName("default"), _realName("default"), _mode_i(true), _kick(false),  _pass(false), _connected(true), _hostname(ipStr), _sock(sock), _saddr_in(sin), _pollFd(pollFd), _nick_registered(false), _user_registered(false)
{
	std::cout << GREEN << "Client " << BGREEN << this->_hostname << GREEN << " connected." << RESET << std::endl;
}

Client::Client()
{
	
}
