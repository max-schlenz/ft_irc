#include "Server.hpp"
#include <iomanip>
#include <algorithm>


//:NICK!USER@HOST PART #channelname :optional_part_message
void Server::part(std::vector<std::string> reqVec, Client& client)
{
	// if (checkPart(reqVec, client, this->_channels))
	if (reqVec.size() > 1)
	{
		std::map<std::string, Channel*>::iterator itChannel = client.getJoinedChannels().find(reqVec[1]);
		
		if (itChannel != client.getJoinedChannels().end())
		{
			Channel* channel = itChannel->second;

			std::string response = ":" + client.getNickname() + "!~" + client.getUsername() + "@127.0.0.1 PART " + reqVec[1] + "\r\n";
			send(client.getSock(), response.c_str(), response.size(), 0);
			this->sendMsgToAll(client, response);
			
			std::map<std::string, Client*>::iterator itClient = channel->getClientsM().find(client.getNickname());
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

//:NickName!UserName@host JOIN #channelname
void Server::join(std::vector<std::string> reqVec, Client& client)
{
	if (reqVec.size() > 1)
	{
		// for (std::vector<Channel*>::iterator it = client.getJoinedChannels().begin(); it != client.getJoinedChannels().end(); ++it)
		// {
		// 	if ((*it)->getName() == reqVec[1])
		// 		return ;
		// }

		// std::vector<Channel>::iterator itChannel = std::find(this->_channels.begin(), this->_channels.end(), )

		std::map<std::string, Channel>::iterator itChannel = this->_channelsM.find(reqVec[1]);

		if (itChannel != this->_channelsM.end())
		{
			client.getJoinedChannels()[reqVec[1]] = &itChannel->second;
			// (*itChannel).getClients().push_back(&client);
			itChannel->second.getClientsM()[client.getNickname()] = &client;

			std::cout << GRAY << client.getNickname() << " joined channel: " << RESET << itChannel->second.getName() << std::endl;
			
			std::string response = ":" + client.getNickname() + "!~" + client.getUsername() + "@localhost JOIN " + reqVec[1] + "\r\n";
			send(client.getSock(), response.c_str(), response.size(), 0);

			this->sendMsgToAll(client, response);
			for (std::vector<Client>::iterator itClient = this->_clients.begin(); itClient != this->_clients.end(); ++itClient)
				this->sendUserList(*itClient, itChannel->second);

			return ;

		}
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

// 311 -> found
// 401 -> not found
void Server::whois(std::vector<std::string> reqVec, Client& client)
{
	if (reqVec.size() > 1 && this->isValidClient(reqVec[1]))
	{
		Client target = this->getClientName(reqVec[1]);
		std::string response = ":127.0.0.1 311 " + client.getNickname() + " " + reqVec[1] + " " + target.getNickname() + " " + target.getUsername() + " * :" + target.getRealName() + "\r\n";
		send(client.getSock(), response.c_str(), response.size(), 0);
		return ;
	}
	else if (reqVec.size() > 1)
	{
		std::string response = ":127.0.0.1 401 " + client.getNickname() + " " + reqVec[1] + " :No such nick/channel\r\n";
		send(client.getSock(), response.c_str(), response.size(), 0);
	}
	else
	{
		std::string response = ":127.0.0.1 311 " + client.getNickname() + " " + reqVec[1] + client.getNickname() + " " + client.getUsername() + " * :" + client.getRealName() + "\r\n";
		send(client.getSock(), response.c_str(), response.size(), 0);
	}
}

void Server::capreq(std::vector<std::string> reqVec, Client &client)
{
	// if (reqVec.size() > 1)
	// {
		std::string response = ":127.0.0.1 CAP * LS :multi-prefix away-notify account-notify\r\n";
		send(client.getSock(), response.c_str(), response.size(), 0);
	// }
	std::cout << GRAY;
	for (std::vector<std::string>::iterator it = reqVec.begin(); it != reqVec.end(); ++it)
		std::cout << *it + " ";
	std::cout << RESET << std::endl;
}

//:NICK!USER@host PRIVMSG <recipient> :<text>
//PRIVMSG <recipient>{,<recipient>} <text to be sent>
void Server::privmsg(std::vector<std::string> reqVec, Client &client)
{
	// for (std::vector<std::string>::iterator it = reqVec.begin(); it != reqVec.end(); ++it)
	// 	std::cout << ">" << *it << std::endl;
	
	if (reqVec.size() > 2)
	{
		for (std::vector<Client>::iterator itClient = this->_clients.begin(); itClient != this->_clients.end(); ++itClient)
		{
			if ((*itClient).getNickname() != client.getNickname() && isUserInChannel(*itClient, reqVec[1]))
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
				send(itClient->getSock(), response.c_str(), response.size(), 0);
			}
		}
	}
} 

void Server::leave(std::vector<std::string> reqVec, Client& client)
{
	std::cout << "leave" << std::endl;
}

//:oldNick!~UserName@host NICK newNick
void Server::nick(std::vector<std::string> reqVec, Client& client)
{
	if (checkNick(reqVec, client))
	{
		std::string oldNick = client.getNickname();
		client.setNickname(reqVec[1]);

		std::string response = ":" + oldNick + "!" + client.getUsername() + "@127.0.0.1 NICK " + reqVec[1] + "\r\n";
		send(client.getSock(), response.c_str(), response.size(), 0);

		std::cout << oldNick << GRAY << " NICK set to " << reqVec[1] << RESET << std::endl;
		
		if (!client.getRegistered())
		{
			std::string response = ":127.0.0.1 001 " + client.getNickname() + " :welcome, " + client.getNickname() + "!" + client.getUsername() + "@" + "127.0.0.1\r\n";
			send(client.getSock(), response.c_str(), response.size(), 0);
			client.setRegistered(true);
			this->_clientsM[client.getNickname()] = &client;
		}
	}
}

// 352 response for each user matching channel
// 315 end of /WHO
// 401 error ERR_NOSUCHNICK
void Server::who(std::vector<std::string> reqVec, Client& client)
{
	// if (reqVec.size() == 2)
	// {
	// 	for (std::vector<Channel>::iterator itChannel = this->_channels.begin(); itChannel != this->_channels.end(); ++itChannel)
	// 	{
	// 		for (std::vector<Client*>::iterator itClient = itChannel->getClients().begin(); itClient != itChannel->getClients().end(); ++itClient)
	// 		{
	// 			std::string response = ":" + client.getNickname() + "!~" + client.getUsername() + "@127.0.0.1 352 " + client.getNickname() + " " 
	// 				+ itChannel->getName() + " " + (*itClient)->getUsername() + "@" + (*itClient)->getHostname() + " " + (*itClient)->getHostname() + " " + (*itClient)->getNickname() 
	// 				+ " H :0 " + (*itClient)->getRealName() + "\r\n";

	// 			send(client.getSock(), response.c_str(), response.size(), 0);
	// 		}
	// 		std::string response = ":" + client.getNickname() + "!~" + client.getUsername() + "@127.0.0.1 315 " + client.getNickname() + " " + reqVec[1] + " :End of /WHO list. \r\n";
	// 		send(client.getSock(), response.c_str(), response.size(), 0);
	// 		return ;
	// 	}
	// }
	// else
	// {
	// 	for (std::vector<Channel>::iterator itChannel = this->_channels.begin(); itChannel != this->_channels.end(); ++itChannel)
	// 	{
	// 		if (itChannel->getName() == reqVec[1])
	// 		{
	// 			for (std::vector<Client*>::iterator itClient = itChannel->getClients().begin(); itClient != itChannel->getClients().end(); ++itClient)
	// 			{
	// 				std::string response = ":" + client.getNickname() + "!~" + client.getUsername() + "@127.0.0.1 352 " + client.getNickname() + " " + itChannel->getName() + " " + (*itClient)->getUsername() + "@" + (*itClient)->getHostname() + " " + (*itClient)->getHostname() + " " + (*itClient)->getNickname() + " H :0 " + (*itClient)->getRealName() + "\r\n";
    //             	send(client.getSock(), response.c_str(), response.size(), 0);
	// 			}
	// 			std::string response = ":" + client.getNickname() + "!~" + client.getUsername() + "@127.0.0.1 315 " + client.getNickname() + " " + reqVec[1] + " :End of /WHO list. \r\n";
    //             send(client.getSock(), response.c_str(), response.size(), 0);
	// 			return ;
	// 		}
	// 	}
	// }
	// if (reqVec.size() > 1)
	// {
	// 	std::string response = ":" + client.getNickname() + "!~" + client.getUsername() + "@127.0.0.1 401 " + client.getNickname() + " " + reqVec[1] + " :No such channel\r\n";
	// 	send(client.getSock(), response.c_str(), response.size(), 0);
	// }
}

void Server::msg(std::vector<std::string> reqVec, Client& client)
{
	if (VERBOSE)
		std::cout << client.getNickname() << GRAY << " msg" << std::endl;
}


// /TOPIC #channelname :New topic	- set topic
// /TOPIC #channelname	- get topic
//
//:nickname!~username@hostname TOPIC #channelname :New topic\r\n
void Server::topic(std::vector<std::string> reqVec, Client& client)
{
	// if (reqVec.size() > 2)
	// {
	// 	for (std::vector<Channel>::iterator itChannel = this->_channels.begin(); itChannel != this->_channels.end(); ++itChannel)
	// 	{
	// 		if (itChannel->getName() == reqVec[1])
	// 		{
	// 			itChannel->setTopic(reqVec[2]);
	// 			std::string response = ":" + client.getNickname() + "!~" + client.getUsername()  + "@127.0.0.1 TOPIC " + itChannel->getName() + " :";
	// 			for (std::vector<std::string>::iterator request = reqVec.begin() + 2; request != reqVec.end(); ++request)
	// 			{
	// 				response += *request;
	// 				if (request + 1 == reqVec.end())
	// 					response += " ";
	// 				else
	// 					response += "\r\n";
	// 			}
	// 			for (std::vector<Client*>::iterator itClient = itChannel->getClients().begin(); itClient != itChannel->getClients().end(); ++itClient)
	// 				send((*itClient)->getSock(), response.c_str(), response.size(), 0);
	// 		}
	// 	}
	// }
	// else if (reqVec.size() == 2)
	// {
	// 	for (std::vector<Channel>::iterator it = this->_channels.begin(); it != this->_channels.end(); ++it)
	// 	{
	// 		if (it->getName() == reqVec[1])
	// 		{
	// 			std::string response = ":" + client.getNickname() + "!~" + client.getUsername() + "@127.0.0.1 332 " + client.getNickname() + " " + reqVec[1] + " :" + it->getTopic() + "\r\n";
	// 			send(client.getSock(), response.c_str(), response.size(), 0);
	// 		}
	// 	}
	// }
	// if (VERBOSE)
	// 	std::cout << client.getNickname() << GRAY << " topic" << std::endl;
}

void Server::mode(std::vector<std::string> reqVec, Client& client)
{
	if (VERBOSE)
		std::cout << client.getNickname() << GRAY << " mode" << std::endl;
}

//KICK #channelname nickname :Kick message
//:nickname!~username@hostname KICK #channelname nickname :Kick message\r\n
//403 channel not existing
//441 not on that channel
void Server::kick(std::vector<std::string> reqVec, Client& client)
{
	// if (reqVec.size() > 2)
	// {
	// 	for (std::vector<Channel>::iterator itChannel = this->_channels.begin(); itChannel != this->_channels.end(); ++itChannel)
	// 	{
	// 		if (itChannel->getName() == reqVec[1])
	// 		{
	// 			for (std::vector<Client*>::iterator itClient = itChannel->getClients().begin(); itClient != itChannel->getClients().end(); ++itClient)
	// 			{
	// 				if ((*itClient)->getNickname() == reqVec[2])
	// 				{
	// 					std::string response = ":" + client.getNickname() + "!~" + client.getUsername() + "@127.0.0.1 KICK " + itChannel->getName() + " " + (*itClient)->getNickname() + " :Kick message\r\n";
	// 					for (std::vector<Client*>::iterator itClient2 = itChannel->getClients().begin(); itClient2 != itChannel->getClients().end(); ++itClient2)
	// 						send((*itClient2)->getSock(), response.c_str(), response.size(), 0);
	// 					itClient = itChannel->getClients().erase(itClient);
	// 					return ;
	// 				}
	// 			}
	// 			std::string response = "441 " + client.getNickname() + " " + reqVec[2] + " " + reqVec[1] + " :They aren't on that channel\r\n";
	// 			send(client.getSock(), response.c_str(), response.size(), 0);
	// 			return ;
	// 		}
	// 	}
	// 	std::string response = ":" + client.getNickname() + "!~" + client.getUsername() + "@127.0.0.1 403 " + client.getNickname() + " " + reqVec[2] + " " + reqVec[1] + " :No such channel\r\n";
	// 	send(client.getSock(), response.c_str(), response.size(), 0);
	// }
}

//NOTICE <nickname|channel> :<message>
//
void Server::notice(std::vector<std::string> reqVec, Client& client)
{
	// if (reqVec.size() > 2)
	// {
	// 	std::string response =  ":" + client.getNickname() + "!~" + client.getUsername() + "@127.0.0.1 NOTICE " + reqVec[1] + " ";
	// 	for (std::vector<std::string>::iterator it = reqVec.begin() + 2; it != reqVec.end(); ++it)
	// 	{
	// 		response += *it;
	// 		if (it + 1 != reqVec.end())
	// 			response += " ";
	// 		else
	// 			response += "\r\n";
	// 	}
	// 	for (std::vector<Client>::iterator itClient = this->_clients.begin(); itClient != this->_clients.end(); ++itClient)
	// 	{
	// 		if (itClient->getNickname() == reqVec[1])
	// 		{
	// 			send(itClient->getSock(), response.c_str(), response.size(), 0);
	// 			return ;
	// 		}
	// 	}
	// 	for (std::vector<Channel>::iterator itChannel = this->_channels.begin(); itChannel != this->_channels.end(); ++itChannel)
	// 	{
	// 		if (itChannel->getName() == reqVec[1])
	// 		{
	// 			for (std::vector<Client*>::iterator itChanClient = itChannel->getClients().begin(); itChanClient != itChannel->getClients().end(); ++itChanClient)
	// 				send((*itChanClient)->getSock(), response.c_str(), response.size(), 0);
	// 			return ;
	// 		}
	// 	}
	// }
}

void Server::invite(std::vector<std::string> reqVec, Client& client)
{
	// if (checkInvite(reqVec, client, this->_channels, *this))
	// 	std::cout << client.getNickname() << GRAY << " invite" << std::endl;
}

//USER max 0 127.0.0.1 :1,11KVIrc 5.0.0 Aria http://kvirc.net/
/*
recVec[0] == USER
recVec[1] == max
recVec[2] == 0
recVec[3] == 127.0.0.1
recVec[4] == :1,11KVIrc
recVec[5] == 5.0.0
recVec[6] == Aria
recVec[7] == http://kvirc.net/
*/
void Server::user(std::vector<std::string> reqVec, Client& client)
{
	if (checkUser(reqVec, client))
	{
		std::cout << client.getNickname() << GRAY << " USER set to " << reqVec[1] << RESET << std::endl;
		client.setUsername(reqVec[1]);

		std::string realName = (*(reqVec.begin() + 4)).substr(1);
		for (std::vector<std::string>::iterator it = reqVec.begin() + 5; it != reqVec.end(); ++it)
			realName += *it;

		client.setRealname(realName);
	}
}

void Server::ping(std::vector<std::string> reqVec, Client& client)
{
	if (reqVec.size() > 1)
	{
		std::string response = "PONG " + reqVec[1] + "\r\n";
		send(client.getSock(), response.c_str(), response.size(), 0);
		// std::cout << client.getNickname() << GRAY << " PING recieved (" << reqVec[1] << ")" << RESET << std::endl;
	}
}

void Server::dbgPrint(std::vector<std::string> reqVec, Client& client)
{
	// std::vector<Client>::iterator clientIt = this->_clients.begin();
	// std::vector<Channel>::iterator channelIt = this->_channels.begin();

	// int numClients = 0;
	// int numChannels = 0;

	// std::cout << BBLUE
	// << "┌────────────┐   ┌────────────┐" << std::endl
	// << "│   USERS    │   │  CHANNELS  │" << std::endl
	// << "├────────────┤   ├────────────┤" << std::endl;

	// while (clientIt != this->_clients.end() || channelIt != this->_channels.end())
	// {
	// 	std::cout << "│ " << RESET;
	// 	if (clientIt != this->_clients.end())
	// 	{
	// 		std::cout << std::setw(11) << std::left << clientIt->getNickname();
	// 		++clientIt;
	// 		++numClients;
	// 	}
	// 	else
	// 		std::cout << std::setw(11) << "";
		
	// 	std::cout << BBLUE << "│   │ " << RESET;

	// 	if (channelIt != this->_channels.end())
	// 	{
	// 		std::cout << std::setw(11) << std::left << channelIt->getName();
	// 		++channelIt;
	// 		++numChannels;
	// 	}
	// 	else
	// 		std::cout << std::setw(11) << "";

	// 	std::cout << BBLUE << "│" << std::endl;
	// }

	// std::cout << BBLUE
	// << "└────────────┘   └────────────┘" << RESET << std::endl;
	// std::cout << " " << numClients << " user(s)" << "        " << numChannels << " channel(s)" << std::endl;
}
