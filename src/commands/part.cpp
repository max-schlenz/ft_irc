#include "Server.hpp"

//: NICK!USER@HOST PART #channelname :optional_part_message
void Server::part(std::vector<std::string> reqVec, Client &client)
{
	if (this->checkPart(reqVec, client))
	{
		std::vector<std::string> partChannels;
		std::string response;
		createLst(reqVec[1], partChannels);
		for (std::vector<std::string>::iterator itPartChannels = partChannels.begin(); itPartChannels != partChannels.end(); ++itPartChannels)
		{
			std::map<std::string, Channel *>::iterator itChannel = client.getJoinedChannels().find(*itPartChannels);
			//! needs to iterate over list of channels to part
	// std::string channelToPart = reqVec[1];
			if (this->_channelsM.find(*itPartChannels) == this->_channelsM.end()) {
				response = E_NOSUCHCHANNEL(client, *itPartChannels);
				this->sendResponse(client, response);
				continue;
			}
			if (client.getJoinedChannels().find(*itPartChannels) == client.getJoinedChannels().end()) {
				response = E_NOTONCHANNEL(client, *itPartChannels);
				this->sendResponse(client, response);
				continue;
			}
			if (itChannel != client.getJoinedChannels().end())
			{
				Channel *channel = itChannel->second;

				std::string response = PART(client, *itPartChannels);
				this->sendResponse(client, response);

				std::map<std::string, Client *>::iterator itClient = channel->getClientsM().find(client.getNickname());
				if (itClient != itChannel->second->getClientsM().end())
				{
					this->sendMsgToAllInChannel(*itChannel->second, response, client.getNickname());
					channel->getClientsM().erase(itClient->second->getNickname());
				}
				size_t numClients = client.getJoinedChannels()[*itPartChannels]->getClientsM().size();

				client.getJoinedChannels().erase(*itPartChannels);

				if (!numClients)
					this->_channelsM.erase(channel->getName());
			}
		}
	}
}
