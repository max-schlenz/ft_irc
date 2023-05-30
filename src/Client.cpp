#include "Client.hpp"
#include "Server.hpp"

std::string Client::getRealName()
{
	return this->_realName;
}

Client::Client(sockaddr_in sin, int sock, std::string ipStr, pollfd pollFd)
	: _pollFd(pollFd), _saddr_in(sin), _sock(sock), _hostname(ipStr), _nickName("default"), _userName("default"), _realName("default"), _kick(false), _nick_registered(false), _user_registered(false), _mode_i(true), _pass(false)
{
	std::cout << GREEN << "Client " << BGREEN << this->_hostname << GREEN << " connected." << RESET << std::endl;
}

Client::Client()
{
	;
	// std::cout << GREEN << "Client DEFAULT CONST" RESET << std::endl;
}

// void Client::createBot(){
// 	int sock;
// 	int port = 6667;

// 	this->_nickName = "bot";
// 	this->_userName = "bot";
// 	this->_realName = "bot";
// 	this->_kick = false;
// 	if(((this->_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0))
// 		error_handling("[-] Error, socket invalid!");
// 	memset(&this->_saddr_in, 0, sizeof(this->_saddr_in));
// 	this->_saddr_in.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
// 	this->_saddr_in.sin_family = AF_INET;
// 	this->_saddr_in.sin_port = htons(port);
// 	this->_hostname = inet_ntoa(this->_saddr_in.sin_addr);
// 	if (connect(this->_sock, (sockaddr*)&this->_saddr_in, sizeof(this->_saddr_in)) == -1)
// 	{
// 		std::cout << RED << "[-] Error, failed connecting the bot!" << RESET << std::flush;
// 		close(this->_sock);
// 	}
// 	fcntl(this->_sock, F_SETFL, O_NONBLOCK);
// 	std::cout << GREEN << "Bot connected." << RESET << std::endl;
// }
