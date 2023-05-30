#include "DiceBot.hpp"
# include <string>
# include <sstream>

extern bool g_run;

static std::string itos(const int i)
{
	std::ostringstream oss;
	oss << i;

	return oss.str();
}

DiceBot::DiceBot(std::string ip, int port, std::string channel):
	_channel(channel)
{
	std::cout << "DiceBot constructed" << std::endl;
	this->_sock = socket(AF_INET, SOCK_STREAM, 0);
	this->_hostname.sin_family = AF_INET;
	this->_hostname.sin_port = htons(port);
	this->_hostname.sin_addr.s_addr = inet_addr(ip.c_str());

	this->_pollFds[0].fd = this->_sock;
	this->_pollFds[0].events = POLLIN;

	connect(this->_sock, (struct sockaddr*)&this->_hostname, sizeof(this->_hostname));
}

DiceBot::~DiceBot()
{
	std::cout << "DiceBot destructed" << std::endl;
	close(this->_sock);
}

int DiceBot::getSock()
{
	return this->_sock;
}

void DiceBot::registerWithServer()
{
    std::string user = "dicebot";
    std::string nickname = "DiceBot";

    std::string req_nick = "NICK " + nickname + "\r\n";
	std::string req_user = "USER " + user + " 0 * :" + user + "\r\n";
	std::string req_join = "JOIN " + this->_channel + "\r\n";

    send(this->getSock(), req_nick.c_str(), req_nick.size(), 0);
    send(this->getSock(), req_user.c_str(), req_user.size(), 0);
    send(this->getSock(), req_join.c_str(), req_join.size(), 0);
}

bool DiceBot::listenToServer()
{
	std::vector<char> buffer(RECV_BUF);

	while (g_run)
	{
		int retPoll = poll(this->_pollFds, 1, 1000);
		if (retPoll < 0)
			break ;
		else if (retPoll > 0 && this->_pollFds[0].revents & POLLIN)
		{
			std::fill(buffer.begin(), buffer.end(), '\0');
			int recv_len = recv(this->_sock, buffer.data(), RECV_BUF, 0);

			if (recv_len <= 0)
				break ;
			else if (recievedRoll(buffer.data()))
				std::cout << buffer.data() << std::endl;
		}
	}
	std::string message = "QUIT :DiceBot leaving\r\n";
	send(this->_sock, message.c_str(), message.size(), 0);
	return true;
}

int DiceBot::roll(int n)
{
    srand(time(NULL)); 
    return (rand() % n) + 1;
}

int DiceBot::recievedRoll(std::string request)
{
	size_t posMessage = request.find_last_of(':');

	if (request.find("!roll ", posMessage) != std::string::npos)
	{
		std::string message = request.substr(posMessage + 1);
        size_t posRoll = message.find("!roll ");
		if (posRoll != std::string::npos)
		{
			std::string maxRoll = message.substr(posRoll + 6);
			int random = this->roll(std::atoi(maxRoll.c_str()));
			std::string response = "PRIVMSG " + this->_channel + " : >> ROLLED: " + itos(random) + "<<\r\n";
			send(this->_sock, response.c_str(), response.size(), 0);
		}
	}
	return 0;
}
