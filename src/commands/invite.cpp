#include "Server.hpp"

void Server::invite(std::vector<std::string> reqVec, Client &client)
{
	std::string response;
	if (this->checkInvite(reqVec, client))
	{
		if (reqVec.size() > 2)
		{
			for (std::map<std::string, Client *>::iterator itClient = this->_clientsM.begin(); itClient != this->_clientsM.end(); ++itClient)
			{
				if (itClient->first == reqVec[1])
				{
					response = R_SENDINV(client, reqVec[1], reqVec[2]);
					this->sendResponse(*itClient->second, response);
					response = R_INVITING(client, reqVec[1], reqVec[2]);
					this->sendResponse(client, response);
				}
			}
		}
	}
}
