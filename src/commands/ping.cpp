#include "Server.hpp"

void Server::ping(std::vector<std::string> reqVec, Client &client)
{
	if (reqVec.size() > 1)
	{
		std::string response = "PONG " + reqVec[1] + "\r\n";
		send(client.getSock(), response.c_str(), response.size(), 0);
		// std::cout << client.getNickname() << GRAY << " PING recieved (" << reqVec[1] << ")" << RESET << std::endl;
	}
}