#include "Server.hpp"

void Server::kick(std::vector<std::string> reqVec, Client &client)
{
	std::string response;
	if (checkKick(reqVec, client))
	{
		std::map<std::string, Channel>::iterator itChannel = this->_channelsM.find(reqVec[1]);
		std::map<std::string, Client*>::iterator itClient = itChannel->second.getClientsM().find(reqVec[2]);
		response = E_KICK(client, itChannel->second.getName(), itClient->second->getNickname());
		for (std::vector<std::string>::iterator it = reqVec.begin() + 3; it != reqVec.end(); ++it)
		{
			response += *it;
			if (it + 1 != reqVec.end())
				response += " ";
			else
				response += "\r\n";
		}
		for (std::map<std::string, Client*>::iterator itClient2 = itChannel->second.getClientsM().begin(); itClient2 != itChannel->second.getClientsM().end(); ++itClient2)
			this->sendResponse(*itClient2->second, response);
		itChannel->second.getClientsM().erase(itClient);
		itClient->second->getJoinedChannels().erase(itChannel->second.getName());
		return ;
	}
}