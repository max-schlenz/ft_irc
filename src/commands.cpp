#include "Server.hpp"

void join(std::vector<std::string> reqVec, Client& client)
{
	std::cout << "JOIN" << std::endl;
}

void leave(std::vector<std::string> reqVec, Client& client)
{
	std::cout << "leave" << std::endl;
}

void nick(std::vector<std::string> reqVec, Client& client)
{
	if (reqVec.size() > 1)
	{
		client.setNickname(reqVec[1]);
		std::cout << GRAY << "NICK set to " << client.getNickname() << std::endl;
	}
}

void msg(std::vector<std::string> reqVec, Client& client)
{
	std::cout << "msg" << std::endl;
}

void topic(std::vector<std::string> reqVec, Client& client)
{
	std::cout << "topic" << std::endl;
}

void mode(std::vector<std::string> reqVec, Client& client)
{
	std::cout << "mode" << std::endl;
}

void kick(std::vector<std::string> reqVec, Client& client)
{
	std::cout << "kick" << std::endl;
}

void invite(std::vector<std::string> reqVec, Client& client)
{
	std::cout << "invite" << std::endl;
}

void user(std::vector<std::string> reqVec, Client& client)
{
	if (reqVec.size() > 1)
	{
		client.setUsername(reqVec[1]);
		std::cout << GRAY << "USER set to " << client.getUsername() << std::endl;
	}
}

void ping(std::vector<std::string> reqVec, Client& client)
{
	if (reqVec.size() > 1)
	{
		std::string response = "PONG " + reqVec[1] + "\r\n";
		send(client.getSock(), response.c_str(), response.size(), 0);
		std::cout << GRAY << "PING recieved from " << client.getIpStr() << " (" << client.getNickname() << ")" << std::endl;
	}
}

