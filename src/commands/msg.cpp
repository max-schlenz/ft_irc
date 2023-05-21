#include "Server.hpp"

void Server::msg(std::vector<std::string> reqVec, Client &client)
{
	if (VERBOSE)
		std::cout << client.getNickname() << GRAY << " msg" << std::endl;
}
