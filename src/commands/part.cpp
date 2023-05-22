#include "Server.hpp"

//: NICK!USER@HOST PART #channelname :optional_part_message
void Server::part(std::vector<std::string> reqVec, Client &client)
{
	if (checkPart(reqVec, client, this->_channelsM, this->_hostname))
	{
		std::map<std::string, Channel *>::iterator itChannel = client.getJoinedChannels().find(reqVec[1]);

		if (itChannel != client.getJoinedChannels().end())
		{
			Channel *channel = itChannel->second;

			std::string response = ":" + client.getNickname() + "!~" + client.getUsername() + "@127.0.0.1 PART " + reqVec[1] + "\r\n";
			send(client.getSock(), response.c_str(), response.size(), 0);
			this->sendMsgToAll(client, response);

			std::map<std::string, Client *>::iterator itClient = channel->getClientsM().find(client.getNickname());
			if (itClient != itChannel->second->getClientsM().end())
				channel->getClientsM().erase(itClient->second->getNickname());

			size_t numClients = client.getJoinedChannels()[reqVec[1]]->getClientsM().size();

			client.getJoinedChannels().erase(reqVec[1]);

			if (numClients == 0)
			{
				this->_channelsM.erase(channel->getName());
				std::cout << GRAY << "removed channel: " << RESET << reqVec[1] << std::endl;
			}
		}
	}
}