#include "Server.hpp"

void Server::capreq(std::vector<std::string> reqVec, Client &client)
{
	if (reqVec.size() > 1)
	{
		if (reqVec[1] == "LS")
			this->sendResponse(client, ":127.0.0.1 CAP * LS :multi-prefix away-notify account-notify\r\n");
		else if (reqVec[1] == "REQ")
			this->sendResponse(client, ":127.0.0.1 CAP * ACK :multi-prefix away-notify account-notify\r\n");
	}
}
