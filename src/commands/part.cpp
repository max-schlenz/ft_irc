#include "Server.hpp"

//: NICK!USER@HOST PART #channelname :optional_part_message
void Server::part(std::vector<std::string> reqVec, Client &client)
{
	if (this->checkPart(reqVec, client))
	{
		std::map<std::string, Channel *>::iterator itChannel = client.getJoinedChannels().find(reqVec[1]);

		if (itChannel != client.getJoinedChannels().end())
		{
			Channel *channel = itChannel->second;

			std::string response = ":" + client.getNickname() + "!~" + client.getUsername() + "@127.0.0.1 PART " + reqVec[1] + "\r\n";
			this->sendResponse(client, response);
			this->sendMsgToAll(client, response);

			std::map<std::string, Client *>::iterator itClient = channel->getClientsM().find(client.getNickname());
			if (itClient != itChannel->second->getClientsM().end())
				channel->getClientsM().erase(itClient->second->getNickname());

			size_t numClients = client.getJoinedChannels()[reqVec[1]]->getClientsM().size();

			client.getJoinedChannels().erase(reqVec[1]);

			if (!numClients)
				this->_channelsM.erase(channel->getName());
		}
	}
}