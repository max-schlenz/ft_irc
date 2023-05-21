#include "Server.hpp"

void Server::mode(std::vector<std::string> reqVec, Client &client)
{
	if (VERBOSE)
		std::cout << client.getNickname() << GRAY << " mode" << std::endl;
}
