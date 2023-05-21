#include "Server.hpp"

//: NICK!USER@host PRIVMSG <recipient> :<text>
// PRIVMSG <recipient>{,<recipient>} <text to be sent>
void Server::privmsg(std::vector<std::string> reqVec, Client &client)
{
	if (reqVec.size() > 2)
	{
		for (std::map<std::string, Client *>::iterator itClient = this->_clientsM.begin(); itClient != this->_clientsM.end(); ++itClient)
		{
			if (itClient->first != client.getNickname() && isUserInChannel(*itClient->second, reqVec[1]))
			{
				std::string response = ":" + client.getNickname() + "!" + client.getUsername() + "@127.0.0.1 PRIVMSG " + reqVec[1] + " :";
				if (reqVec[2].length() > 1)
					reqVec[2] = reqVec[2].substr(1);
				for (std::vector<std::string>::iterator itVec = reqVec.begin() + 2; itVec != reqVec.end(); ++itVec)
				{
					response += *itVec;
					if (itVec + 1 != reqVec.end())
						response += " ";
				}
				response += "\r\n";
				send(itClient->second->getSock(), response.c_str(), response.size(), 0);
			}
		}
	}
}
