#include "Server.hpp"

//: NICK!USER@host PRIVMSG <recipient> :<text>
// PRIVMSG <recipient>{,<recipient>} <text to be sent>
//
//PRIVMSG User1 :DCC SEND file 2130706433 60052 4
//
void Server::privmsg(std::vector<std::string> reqVec, Client &client)
{
	if (reqVec.size() > 3 && reqVec[2] == ":\1DCC" && reqVec[3] == "SEND")
		return this->dcc(reqVec, client);
	else if (reqVec.size() > 2)
	{
		std::vector<std::string> recipients;
		createLst(reqVec[1], recipients);
		for (std::vector<std::string>::iterator itRecipient = recipients.begin(); itRecipient != recipients.end(); ++itRecipient)
		{
			if (*itRecipient->begin() == '#')
			{
				std::map<std::string, Channel>::iterator itRecChan = this->_channelsM.find(*itRecipient);
				if (itRecChan != this->_channelsM.end())
				{
					std::string response = ":" + client.getNickname() + "!" + client.getUsername() + "@127.0.0.1 PRIVMSG " + *itRecipient + " :";
					if (reqVec[2].length() > 1)
						reqVec[2] = reqVec[2].substr(1);
					for (std::vector<std::string>::iterator itVec = reqVec.begin() + 2; itVec != reqVec.end(); ++itVec)
					{
						response += *itVec;
						if (itVec + 1 != reqVec.end())
							response += " ";
					}
					response += "\r\n";
					for (std::map<std::string, Client*>::iterator itRecChanClient = itRecChan->second.getClientsM().begin(); itRecChanClient != itRecChan->second.getClientsM().end(); ++itRecChanClient)
					{
						if (itRecChanClient->first != client.getNickname() && isUserInChannel(*itRecChanClient->second, *itRecipient))
							this->sendResponse(*itRecChanClient->second, response);
					}
				}
				continue;
			}

			std::map<std::string, Client *>::iterator itClient = this->_clientsM.find(*itRecipient);
			if (itClient == this->_clientsM.end())
			{
				this->sendResponse(client, "401 " + client.getNickname() + " " + *itRecipient + " :No such nick/channel\r\n");
				continue;
			}

			if (itClient->first != client.getNickname())
			{
				std::string response = ":" + client.getNickname() + "!" + client.getUsername() + "@127.0.0.1 PRIVMSG " + *itRecipient + " :";
				if (reqVec[2].length() > 1)
					reqVec[2] = reqVec[2].substr(1);
				for (std::vector<std::string>::iterator itVec = reqVec.begin() + 2; itVec != reqVec.end(); ++itVec)
				{
					response += *itVec;
					if (itVec + 1 != reqVec.end())
						response += " ";
				}
				response += "\r\n";
				this->sendResponse(*itClient->second, response);
			}
		}
	}
}
