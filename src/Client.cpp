#include "Client.hpp"
#include "Server.hpp"

Client::Client(sockaddr_in sin, socklen_t sinLen, int sock, char* ipStr, pollfd& pollFd):
	_pollFd(pollFd), _saddr_in(sin), _saddr_in_len(sinLen), _sock(sock), _ipStr(ipStr), _nickName("")
{
	std::cout << GREEN << "Client " << BGREEN << this->_ipStr << GREEN << " connected." << RESET << std::endl;
}
