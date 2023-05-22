#include "Server.hpp"

void Server::invite(std::vector<std::string> reqVec, Client &client)
{
	if (this->checkInvite(reqVec, client))
		std::cout << client.getNickname() << GRAY << " invite" << std::endl;
}
