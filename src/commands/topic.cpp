#include "Server.hpp"

// /TOPIC #channelname :New topic	- set topic
// /TOPIC #channelname	- get topic
//
//: nickname!~username@hostname TOPIC #channelname :New topic\r\n
void Server::topic(std::vector<std::string> reqVec, Client &client)
{
	if (reqVec.size() == 2)
	{
		std::map<std::string, Channel>::iterator it = this->_channelsM.find(reqVec[1]);

		if (it != this->_channelsM.end())
			this->sendResponse(client, ":" + client.getNickname() + "!~" + client.getUsername() + "@127.0.0.1 332 " + client.getNickname() + " " + reqVec[1] + " :" + it->second.getTopic() + "\r\n");
		else
			this->sendResponse(client, ":" + client.getNickname() + "!~" + client.getUsername() + "@127.0.0.1 403 " + client.getNickname() + " " + reqVec[1] + " :No such channel\r\n");
	}
	if (reqVec.size() > 2)
	{
		std::map<std::string, Channel>::iterator itChannel = this->_channelsM.find(reqVec[1]);

		if (itChannel != this->_channelsM.end())
		{
			std::string topic;
			for (std::vector<std::string>::iterator request = reqVec.begin() + 2; request != reqVec.end(); ++request)
			{
				topic += *request;
				if (request + 1 != reqVec.end())
					topic += " ";
			}
			itChannel->second.setTopic(topic);
			std::string response = ":" + client.getNickname() + "!~" + client.getUsername() + "@127.0.0.1 TOPIC " + itChannel->second.getName() + " :" + topic + "\r\n";
			for (std::map<std::string, Client *>::iterator itClient = itChannel->second.getClientsM().begin(); itClient != itChannel->second.getClientsM().end(); ++itClient)
				this->sendResponse(*itClient->second, response);
		}
		else
			this->sendResponse(client, ":" + client.getNickname() + "!~" + client.getUsername() + "@127.0.0.1 403 " + client.getNickname() + " " + reqVec[1] + " :No such channel\r\n");
	}
}