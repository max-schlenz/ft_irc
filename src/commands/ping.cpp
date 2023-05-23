#include "Server.hpp"

void Server::ping(std::vector<std::string> reqVec, Client &client)
{
	if (reqVec.size() > 1)
		this->sendResponse(client, "PONG " + reqVec[1] + "\r\n");
}