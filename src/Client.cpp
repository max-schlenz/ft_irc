#include "Client.hpp"
#include "Server.hpp"

std::string Client::getRealName()
{
	return this->_realName;
}

Client::Client(sockaddr_in sin, int sock, std::string ipStr, pollfd pollFd)
	: _pollFd(pollFd), _saddr_in(sin), _sock(sock), _hostname(ipStr), _nickName("default"), _userName("default"), _realName("default"), _kick(false), _nick_registered(false), _user_registered(false), _mode_i(true), _pass(false), _connected(true)
{
	std::cout << GREEN << "Client " << BGREEN << this->_hostname << GREEN << " connected." << RESET << std::endl;
}
