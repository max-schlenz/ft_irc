#include "Server.hpp"

void Server::pass(std::vector<std::string> reqVec, Client &client)
{
	if (reqVec.size() > 1)
	{
		std::string pass = reqVec[1].replace(0,1,"");
		std::cout << pass << std::endl;
	}
		// this->sendResponse(client, "PONG " + reqVec[1] + "\r\n");
}