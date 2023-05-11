#include "Client.hpp"
#include "Server.hpp"

Client::Client(sockaddr_in sin, socklen_t sinLen, int sock, char* ipStr)
{
	this->_saddr_in = sin;
	this->_saddr_in_len = sinLen;
	this->_sock = sock;
	this->_ipStr = ipStr;
}