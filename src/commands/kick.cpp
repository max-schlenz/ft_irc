#include "Server.hpp"

// KICK #channelname nickname :Kick message
//: nickname!~username@hostname KICK #channelname nickname :Kick message\r\n
// 403 channel not existing
// 441 not on that channel
void Server::kick(std::vector<std::string> reqVec, Client &client)
{
	if (reqVec.size() > 2)
	{
		std::map<std::string, Channel>::iterator itChannel = this->_channelsM.find(reqVec[1]);

		if (itChannel != this->_channelsM.end())
		{
			std::map<std::string, Client*>::iterator itClient = itChannel->second.getClientsM().find(reqVec[2]);
			if (itClient != itChannel->second.getClientsM().end())
			{
				std::string response = ":" + client.getNickname() + "!~" + client.getUsername() + "@127.0.0.1 KICK " + itChannel->second.getName() + " " + itClient->second->getNickname() + " :Kick message\r\n";
				for (std::map<std::string, Client*>::iterator itClient2 = itChannel->second.getClientsM().begin(); itClient2 != itChannel->second.getClientsM().end(); ++itClient2)
					this->sendResponse(*itClient2->second, response);
				itChannel->second.getClientsM().erase(itClient);
				itClient->second->getJoinedChannels().erase(itChannel->second.getName());
				return ;
			}
			else
				return this->sendResponse(client, "441 " + client.getNickname() + " " + reqVec[2] + " " + reqVec[1] + " :They aren't on that channel\r\n");
		}
		else
			this->sendResponse(client, ":" + client.getNickname() + "!~" + client.getUsername() + "@127.0.0.1 403 " + client.getNickname() + " " + reqVec[2] + " " + reqVec[1] + " :No such channel\r\n");
	}
}
