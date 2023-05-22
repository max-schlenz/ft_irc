#include "Server.hpp"

//: NickName!UserName@host JOIN #channelname
void Server::join(std::vector<std::string> reqVec, Client &client)
{
	if (reqVec.size() > 1)
	{
		if (client.getJoinedChannels().find(reqVec[1]) != client.getJoinedChannels().end())
			return;

		std::map<std::string, Channel>::iterator itChannel = this->_channelsM.find(reqVec[1]);

		if (itChannel != this->_channelsM.end())
		{
			client.getJoinedChannels()[reqVec[1]] = &itChannel->second;
			itChannel->second.getClientsM()[client.getNickname()] = &client;

			std::cout << GRAY << client.getNickname() << " joined channel: " << RESET << itChannel->second.getName() << std::endl;

			std::string response = ":" + client.getNickname() + "!~" + client.getUsername() + "@localhost JOIN " + reqVec[1] + "\r\n";

			send(client.getSock(), response.c_str(), response.size(), 0);
			this->sendMsgToAll(client, response);
			for (std::vector<Client>::iterator itClient = this->_clients.begin(); itClient != this->_clients.end(); ++itClient)
				this->sendUserList(*itClient, itChannel->second);

			return;
		}
		else
		{
			Channel channel(reqVec[1]);
			this->_channelsM[reqVec[1]] = channel;
			this->_channelsM[reqVec[1]].getClientsM()[client.getNickname()] = &client;
			client.getJoinedChannels()[reqVec[1]] = &this->_channelsM[reqVec[1]];

			std::cout << GRAY << client.getNickname() << " joined channel: " << RESET << reqVec[1] << std::endl;
			std::string response = ":" + client.getNickname() + "!~" + client.getUsername() + "@localhost JOIN " + reqVec[1] + "\r\n";

			send(client.getSock(), response.c_str(), response.size(), 0);
			this->sendMsgToAll(client, response);
			for (std::vector<Client>::iterator it = this->_clients.begin(); it != this->_clients.end(); ++it)
				this->sendUserList(*it, channel);
		}
	}
}
