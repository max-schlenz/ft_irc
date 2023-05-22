#include "Server.hpp"

void Server::capreq(std::vector<std::string> reqVec, Client &client)
{
	if (reqVec.size() > 1)
	{
		if (reqVec[1] == "LS")
		{
			std::string response = ":127.0.0.1 CAP * LS :multi-prefix away-notify account-notify\r\n";
			send(client.getSock(), response.c_str(), response.size(), 0);
		}
		else if (reqVec[1] == "REQ")
		{
			std::string response = ":127.0.0.1 CAP * ACK :multi-prefix away-notify account-notify\r\n";
			send(client.getSock(), response.c_str(), response.size(), 0);
		}
	}
}
