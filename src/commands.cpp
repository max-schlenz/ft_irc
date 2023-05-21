#include "Server.hpp"
#include <iomanip>
#include <algorithm>

//: NICK!USER@HOST PART #channelname :optional_part_message
void Server::part(std::vector<std::string> reqVec, Client &client)
{
	if (reqVec.size() > 1)
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

// 311 -> found
// 401 -> not found
void Server::whois(std::vector<std::string> reqVec, Client &client)
{
	if (reqVec.size() > 1 && this->isValidClient(reqVec[1]))
	{
		Client target = this->getClientName(reqVec[1]);
		std::string response = ":127.0.0.1 311 " + client.getNickname() + " " + reqVec[1] + " " + target.getNickname() + " " + target.getUsername() + " * :" + target.getRealName() + "\r\n";
		send(client.getSock(), response.c_str(), response.size(), 0);
		return;
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

void Server::leave(std::vector<std::string> reqVec, Client &client)
{
	std::cout << "leave" << std::endl;
}

//: oldNick!~UserName@host NICK newNick
void Server::nick(std::vector<std::string> reqVec, Client &client)
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
void Server::who(std::vector<std::string> reqVec, Client &client)
{
	if (reqVec.size() >= 2)
	{
		std::map<std::string, Channel>::iterator itChannel = this->_channelsM.find(reqVec[1]);

		if (itChannel != this->_channelsM.end()) // channel found
		{
			for (std::map<std::string, Client *>::iterator itClient = itChannel->second.getClientsM().begin(); itClient != itChannel->second.getClientsM().end(); ++itClient)
			{
				std::string response = ":" + client.getNickname() + "!~" + client.getUsername() + "@127.0.0.1 352 " + client.getNickname() + " " + itChannel->second.getName() + " " + itClient->second->getUsername() + "@" + itClient->second->getHostname() + " " + itClient->second->getHostname() + " " + itClient->second->getNickname() + " H :0 " + itClient->second->getRealName() + "\r\n";
				send(client.getSock(), response.c_str(), response.size(), 0);
			}
			std::string response = ":" + client.getNickname() + "!~" + client.getUsername() + "@127.0.0.1 315 " + client.getNickname() + " " + reqVec[1] + " :End of /WHO list. \r\n";
			send(client.getSock(), response.c_str(), response.size(), 0);
			return;
		}
		else
		{
			std::map<std::string, Client *>::iterator itClient = this->_clientsM.find(reqVec[1]);
			if (itClient != this->_clientsM.end())
			{
				std::string response = ":" + client.getNickname() + "!~" + client.getUsername() + "@127.0.0.1 352 " + client.getNickname() + " * " + itClient->second->getUsername() + "@" + itClient->second->getHostname() + " " + itClient->second->getHostname() + " " + itClient->second->getNickname() + " H :0 " + itClient->second->getRealName() + "\r\n";
				send(client.getSock(), response.c_str(), response.size(), 0);
				return;
			}
		}
	}
	else
	{
		std::map<std::string, Channel>::iterator itChannel = this->_channelsM.find(reqVec[1]);

		if (itChannel != this->_channelsM.end())
		{
			for (std::map<std::string, Client *>::iterator itClient = itChannel->second.getClientsM().begin(); itClient != itChannel->second.getClientsM().end(); ++itClient)
			{
				std::string response = ":" + client.getNickname() + "!~" + client.getUsername() + "@127.0.0.1 352 " + client.getNickname() + " " + itChannel->second.getName() + " " + itClient->second->getUsername() + "@" + itClient->second->getHostname() + " " + itClient->second->getHostname() + " " + itClient->second->getNickname() + " H :0 " + itClient->second->getRealName() + "\r\n";
				send(client.getSock(), response.c_str(), response.size(), 0);
			}
			std::string response = ":" + client.getNickname() + "!~" + client.getUsername() + "@127.0.0.1 315 " + client.getNickname() + " " + reqVec[1] + " :End of /WHO list. \r\n";
			send(client.getSock(), response.c_str(), response.size(), 0);
			PRNT;
			return;
		}
	}
	if (reqVec.size() > 1) // if channel not found
	{
		std::string response = ":" + client.getNickname() + "!~" + client.getUsername() + "@127.0.0.1 401 " + client.getNickname() + " " + reqVec[1] + " :No such channel\r\n";
		send(client.getSock(), response.c_str(), response.size(), 0);
	}
}

void Server::msg(std::vector<std::string> reqVec, Client &client)
{
	if (VERBOSE)
		std::cout << client.getNickname() << GRAY << " msg" << std::endl;
}

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
		{
			std::string response = ":" + client.getNickname() + "!~" + client.getUsername() + "@127.0.0.1 332 " + client.getNickname() + " " + reqVec[1] + " :" + it->second.getTopic() + "\r\n";
			send(client.getSock(), response.c_str(), response.size(), 0);
		}
		else
		{
			std::string response = ":" + client.getNickname() + "!~" + client.getUsername() + "@127.0.0.1 403 " + client.getNickname() + " " + reqVec[1] + " :No such channel\r\n";
			send(client.getSock(), response.c_str(), response.size(), 0);		
		}
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
				send(itClient->second->getSock(), response.c_str(), response.size(), 0);
		}
		else
		{
			std::string response = ":" + client.getNickname() + "!~" + client.getUsername() + "@127.0.0.1 403 " + client.getNickname() + " " + reqVec[1] + " :No such channel\r\n";
			send(client.getSock(), response.c_str(), response.size(), 0);		
		}
	}
}

void Server::mode(std::vector<std::string> reqVec, Client &client)
{
	if (VERBOSE)
		std::cout << client.getNickname() << GRAY << " mode" << std::endl;
}

// KICK #channelname nickname :Kick message
//: nickname!~username@hostname KICK #channelname nickname :Kick message\r\n
// 403 channel not existing
// 441 not on that channel
void Server::kick(std::vector<std::string> reqVec, Client &client)
{
	if (reqVec.size() > 2)
	{
		std::map<std::string, Channel>::iterator itChannel = this->_channelsM.find(reqVec[1]);

		if (itChannel != this->_channelsM.end())
		{
			std::map<std::string, Client*>::iterator itClient = itChannel->second.getClientsM().find(reqVec[2]);
			if (itClient != itChannel->second.getClientsM().end())
			{
				std::string response = ":" + client.getNickname() + "!~" + client.getUsername() + "@127.0.0.1 KICK " + itChannel->second.getName() + " " + itClient->second->getNickname() + " :Kick message\r\n";
				for (std::map<std::string, Client*>::iterator itClient2 = itChannel->second.getClientsM().begin(); itClient2 != itChannel->second.getClientsM().end(); ++itClient2)
					send(itClient2->second->getSock(), response.c_str(), response.size(), 0);
				itChannel->second.getClientsM().erase(itClient);
				itClient->second->getJoinedChannels().erase(itChannel->second.getName());
				return ;
			}
			else
			{
				std::string response = "441 " + client.getNickname() + " " + reqVec[2] + " " + reqVec[1] + " :They aren't on that channel\r\n";
				send(client.getSock(), response.c_str(), response.size(), 0);
				return ;
			}
		}
		else
		{
			std::string response = ":" + client.getNickname() + "!~" + client.getUsername() + "@127.0.0.1 403 " + client.getNickname() + " " + reqVec[2] + " " + reqVec[1] + " :No such channel\r\n";
			send(client.getSock(), response.c_str(), response.size(), 0);
		}

	}
}

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
		{
			send(itClient->second->getSock(), response.c_str(), response.size(), 0);
			return ;
		}

		std::map<std::string, Channel>::iterator itChannel = this->_channelsM.find(reqVec[1]);
		if (itChannel != this->_channelsM.end())
		{
			for (std::map<std::string, Client*>::iterator itChanClient = itChannel->second.getClientsM().begin(); itChanClient != itChannel->second.getClientsM().end(); ++itChanClient)
					send(itChanClient->second->getSock(), response.c_str(), response.size(), 0);
			return ;
		}
	}
}

void Server::invite(std::vector<std::string> reqVec, Client &client)
{
	// if (checkInvite(reqVec, client, this->_channels, *this))
	// 	std::cout << client.getNickname() << GRAY << " invite" << std::endl;
}

// USER max 0 127.0.0.1 :1,11KVIrc 5.0.0 Aria http://kvirc.net/
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
void Server::user(std::vector<std::string> reqVec, Client &client)
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

void Server::ping(std::vector<std::string> reqVec, Client &client)
{
	if (reqVec.size() > 1)
	{
		std::string response = "PONG " + reqVec[1] + "\r\n";
		send(client.getSock(), response.c_str(), response.size(), 0);
		// std::cout << client.getNickname() << GRAY << " PING recieved (" << reqVec[1] << ")" << RESET << std::endl;
	}
}

void Server::dbgPrint(std::vector<std::string> reqVec, Client &client)
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
