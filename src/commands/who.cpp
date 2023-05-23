#include "Server.hpp"

void printReqVec(std::vector<std::string> reqVec)
{
	int i = 0;
	for (std::vector<std::string>::iterator it = reqVec.begin(); it != reqVec.end(); ++it, i++)
		std::cout << i << ":" << *it << std::endl;
}

// 352 response for each user matching channel
// 315 end of /WHO
// 401 error ERR_NOSUCHNICK
void Server::who(std::vector<std::string> reqVec, Client &client)
{
	if (reqVec.size() >= 2)
	{
		std::map<std::string, Channel>::iterator itChannel = this->_channelsM.find(reqVec[1]);

		if (itChannel != this->_channelsM.end()) // channel found
		{
			for (std::map<std::string, Client *>::iterator itClient = itChannel->second.getClientsM().begin(); itClient != itChannel->second.getClientsM().end(); ++itClient)
				this->sendResponse(client, ":" + client.getNickname() + "!~" + client.getUsername() + "@127.0.0.1 352 " + client.getNickname() + " " + itChannel->second.getName() + " " + itClient->second->getUsername() + "@" + itClient->second->getHostname() + " " + itClient->second->getHostname() + " " + itClient->second->getNickname() + " H :0 " + itClient->second->getRealName() + "\r\n");
			return this->sendResponse(client, ":" + client.getNickname() + "!~" + client.getUsername() + "@127.0.0.1 315 " + client.getNickname() + " " + reqVec[1] + " :End of /WHO list. \r\n");
		}
		else
		{
			std::map<std::string, Client *>::iterator itClient = this->_clientsM.find(reqVec[1]);
			if (itClient != this->_clientsM.end())
				return this->sendResponse(*itClient->second, ":" + client.getNickname() + "!~" + client.getUsername() + "@127.0.0.1 352 " + client.getNickname() + " * " + itClient->second->getUsername() + "@" + itClient->second->getHostname() + " " + itClient->second->getHostname() + " " + itClient->second->getNickname() + " H :0 " + itClient->second->getRealName() + "\r\n");
		}
	}
	else
	{
		std::map<std::string, Channel>::iterator itChannel = this->_channelsM.find(reqVec[1]);

		if (itChannel != this->_channelsM.end())
		{
			for (std::map<std::string, Client *>::iterator itClient = itChannel->second.getClientsM().begin(); itClient != itChannel->second.getClientsM().end(); ++itClient)
				this->sendResponse(client, ":" + client.getNickname() + "!~" + client.getUsername() + "@127.0.0.1 352 " + client.getNickname() + " " + itChannel->second.getName() + " " + itClient->second->getUsername() + "@" + itClient->second->getHostname() + " " + itClient->second->getHostname() + " " + itClient->second->getNickname() + " H :0 " + itClient->second->getRealName() + "\r\n");
			return this->sendResponse(client, ":" + client.getNickname() + "!~" + client.getUsername() + "@127.0.0.1 315 " + client.getNickname() + " " + reqVec[1] + " :End of /WHO list. \r\n");
		}
	}
	if (reqVec.size() > 1) // if channel not found
		this->sendResponse(client, ":" + client.getNickname() + "!~" + client.getUsername() + "@127.0.0.1 401 " + client.getNickname() + " " + reqVec[1] + " :No such channel\r\n");
}