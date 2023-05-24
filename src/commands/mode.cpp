#include "Server.hpp"

void Server::mode(std::vector<std::string> reqVec, Client &client)
{
	if (this->checkMode(reqVec, client))
		std::cout << client.getNickname() << GRAY << " mode" << std::endl;
}
