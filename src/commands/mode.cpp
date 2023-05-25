#include "Server.hpp"

static bool isUserMode(std::string channelName)
{
	if (channelName[0] != '#')
		return true;
	return false;
}

void Server::mode(std::vector<std::string> reqVec, Client &client)
{
	if (isUserMode(reqVec[1]) && this->checkUserMode(reqVec, client))
		std::cout << client.getNickname() << GRAY << " mode" << std::endl;
	else if (this->checkChannelMode(reqVec, client)) {
		
	}
}
