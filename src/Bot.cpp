#include "Bot.hpp"
# include "definitions.hpp"

extern bool g_bot;

Bot::Bot(int port, std::string ipStr)
{
	int			sock;

	g_bot = true;
	this->_nickName = "Bot";
	this->_userName = "Bot";
	this->_realName = "Bot";
	this->_kick = false;
	if(((this->_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0))
	{
		std::cout << RED << "[-] Error, bot socket invalid!" << RESET << std::flush;
		g_bot = false;
	}
	else
	{
		memset(&this->_saddr_in, 0, sizeof(this->_saddr_in));
		this->_saddr_in.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
		this->_saddr_in.sin_family = AF_INET;
		this->_saddr_in.sin_port = htons(port);
		this->_hostname = inet_ntoa(this->_saddr_in.sin_addr);
		if (connect(this->_sock, (sockaddr*)&this->_saddr_in, sizeof(this->_saddr_in)) == -1)
		{
			std::cout << RED << "[-] Error, failed connecting the bot!" << RESET << std::flush;
			g_bot = false;
			close(this->_sock);
		}
		else
		{
			fcntl(this->_sock, F_SETFL, O_NONBLOCK);
			std::cout << GREEN << "Bot connected." << RESET << std::endl;
		}

	}
}

Bot::Bot()
{
	int			port = 6667;
	int			sock;
	std::string	ipStr = "127.0.0.1";

	this->_nickName = "Bot";
	this->_userName = "Bot";
	this->_realName = "Bot";
	this->_kick = false;
	if(((this->_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0))
		error_handling("[-] Error, bot socket invalid!");
	memset(&this->_saddr_in, 0, sizeof(this->_saddr_in));
	this->_saddr_in.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	this->_saddr_in.sin_family = AF_INET;
	this->_saddr_in.sin_port = htons(port);
	this->_hostname = inet_ntoa(this->_saddr_in.sin_addr);
	if (connect(this->_sock, (sockaddr*)&this->_saddr_in, sizeof(this->_saddr_in)) == -1)
	{
		std::cout << RED << "[-] Error, failed connecting the bot!" << RESET << std::flush;
		close(this->_sock);
	}
	fcntl(this->_sock, F_SETFL, O_NONBLOCK);
	std::cout << GREEN << "Bot connected." << RESET << std::endl;
}
