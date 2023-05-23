#include "Server.hpp"

//:<server> 353 <nick> = <channel> :<space-separated list of nicks>
void Server::sendUserList(Client& client, Channel& channel)
{
	std::string response = ":127.0.0.1 353 " + client.getNickname() + " = " + channel.getName() + " :";
	for (std::map<std::string, Client*>::iterator it = channel.getClientsM().begin(); it != channel.getClientsM().end(); ++it)
	{
 		response += it->first;
		std::map<std::string, Client*>::iterator itNext = it;
		++itNext;
		if (itNext != channel.getClientsM().end())
			response += " ";
		else
		{
			response += "\r\n";
			send(client.getSock(), response.c_str(), response.size(), 0);
		}
	}
	
	response = ":127.0.0.1 366 " + client.getNickname() + " " + channel.getName() + " :End of /NAMES list\r\n";
	send(client.getSock(), response.c_str(), response.size(), 0);

	// std::cout << BRED << response << RESET << std::endl;
}

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

			std::string response = ":" + client.getNickname() + "!~" + client.getUsername() + "@localhost JOIN " + reqVec[1] + "\r\n";
			this->sendResponse(client, response);

			this->sendMsgToAllInChannel(itChannel->second, response, client);
			// this->sendMsgToAll(client, response);
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

			std::string response = ":" + client.getNickname() + "!~" + client.getUsername() + "@localhost JOIN " + reqVec[1] + "\r\n";
			this->sendResponse(client, response);
			this->sendMsgToAllInChannel(channel, response, client);
			// this->sendMsgToAll(client, response);
			for (std::vector<Client>::iterator it = this->_clients.begin(); it != this->_clients.end(); ++it)
				this->sendUserList(*it, channel);
		}
	}
}
