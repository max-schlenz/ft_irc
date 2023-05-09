#include "Client.hpp"
#include "Server.hpp"

Client::Client(sockaddr_in sin, socklen_t sinLen, int sock, char* ipStr)
{
	this->_sin = sin;
	this->_sinLen = sinLen;
	this->_id = sock;
	this->_ipStr = ipStr;
}