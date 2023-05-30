#include "Server.hpp"

void Server::capreq(std::vector<std::string> reqVec, Client &client)
{
	if (reqVec.size() > 1)
	{
		if (reqVec[1] == "LS")
		{
			std::string response = R_CAP("LS");
			this->sendResponse(client, response);
		}
		else if (reqVec[1] == "REQ")
		{
			std::string response = R_CAP("ACK");
			this->sendResponse(client, response);
		}
	}
}
