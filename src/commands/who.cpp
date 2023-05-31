#include "Server.hpp"

void Server::who(std::vector<std::string> reqVec, Client &client)
{
	std::string response;

	if (reqVec.size() >= 2)
	{
		std::map<std::string, Channel>::iterator itChannel = this->_channelsM.find(reqVec[1]);

		if (itChannel != this->_channelsM.end())
		{
			for (std::map<std::string, Client *>::iterator itClient = itChannel->second.getClientsM().begin(); itClient != itChannel->second.getClientsM().end(); ++itClient)
				this->sendResponse(client, ":" + client.getNickname() + "!~" + client.getUsername() + "@127.0.0.1 352 " + client.getNickname() + " " + itChannel->second.getName() + " " + itClient->second->getUsername() + "@" + itClient->second->getHostname() + " " + itClient->second->getHostname() + " " + itClient->second->getNickname() + " H :0 " + itClient->second->getRealName() + "\r\n");
			return this->sendResponse(client, R_ENDOFWHO(client, reqVec[1]));
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
			{
				response = R_ENDOFWHO(client, reqVec[1]);
				return this->sendResponse(client, response);
			}
		}
	}
	if (reqVec.size() > 1)
		this->sendResponse(client, E_NOSUCHCHANNEL(client, reqVec[1]));
}
