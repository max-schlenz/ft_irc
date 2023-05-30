#include "Server.hpp"

void Server::capreq(std::vector<std::string> reqVec, Client &client)
{
	if (reqVec.size() > 1)
	{
		if (reqVec[1] == "LS")
			return this->sendResponse(client, R_CAP("LS"));
		else if (reqVec[1] == "REQ")
			return this->sendResponse(client, R_CAP("ACK"));
	}
}
