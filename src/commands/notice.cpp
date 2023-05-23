#include "Server.hpp"

// NOTICE <nickname|channel> :<message>
//
void Server::notice(std::vector<std::string> reqVec, Client &client)
{
	if (reqVec.size() > 2)
	{
		std::string response =  ":" + client.getNickname() + "!~" + client.getUsername() + "@127.0.0.1 NOTICE " + reqVec[1] + " ";
		for (std::vector<std::string>::iterator it = reqVec.begin() + 2; it != reqVec.end(); ++it)
		{
			response += *it;
			if (it + 1 != reqVec.end())
				response += " ";
			else
				response += "\r\n";
		}
		std::map<std::string, Client*>::iterator itClient = this->_clientsM.find(reqVec[1]);
		if (itClient != this->_clientsM.end())
			return this->sendResponse(*itClient->second, response);

		std::map<std::string, Channel>::iterator itChannel = this->_channelsM.find(reqVec[1]);
		if (itChannel != this->_channelsM.end())
		{
			for (std::map<std::string, Client*>::iterator itChanClient = itChannel->second.getClientsM().begin(); itChanClient != itChannel->second.getClientsM().end(); ++itChanClient)
					this->sendResponse(*itClient->second, response);
			return ;
		}
	}
}