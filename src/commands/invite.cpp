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
					std::cout << "reqVec: " << reqVec[2] << std::endl;
					this->_channelsM[reqVec[2]].addInvitedClient(reqVec[1], &client);
					this->sendResponse(*itClient->second, R_SENDINV(client, reqVec[1], reqVec[2]));
					this->sendResponse(client, R_INVITING(client, reqVec[1], reqVec[2]));
				}
			}
		}
	}
}
