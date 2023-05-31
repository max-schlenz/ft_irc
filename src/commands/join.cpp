#include "Server.hpp"

void Server::sendUserList(Client& client, Channel& channel)
{
	std::string response = ":127.0.0.1 353 " + client.getNickname() + " = " + channel.getName() + " :";
	for (std::map<std::string, Client*>::iterator it = channel.getClientsM().begin(); it != channel.getClientsM().end(); ++it)
	{
		std::map<std::string, Client*>::iterator itOp = channel.getOperators().find(it->first);
		if (itOp != channel.getOperators().end())
			response += "@";
 		response += it->first;
		std::map<std::string, Client*>::iterator itNext = it;
		++itNext;
		if (itNext != channel.getClientsM().end())
			response += " ";
		else
		{
			response += "\r\n";
			this->sendResponse(client, response);
		}
	}
	response = E_ENDOFNAMES(client, channel.getName());
	this->sendResponse(client, response);
}

bool Server::checkPassword(std::string channelName, std::string password) {
	if (this->_channelsM[channelName].getPassword() != password)
		return false;
	return true;
}

void Server::joinAsOperator(std::string channelName, Client &client)
{
	Channel channel(channelName);
	this->_channelsM[channelName] = channel;
	this->_channelsM[channelName].getClientsM()[client.getNickname()] = &client;
	this->_channelsM[channelName].addOperator(client.getNickname(), &client);
	client.getJoinedChannels()[channelName] = &this->_channelsM[channelName];
	this->_num_channels++;

	std::string response = JOIN(client, channelName);
	this->sendResponse(client, response);
	this->sendMsgToAllInChannel(channel, response, client.getNickname());
	response = CHANNELMODEARGS(client, channelName, "+o", client.getNickname());
	this->sendResponse(client, response);
	this->sendMsgToAllInChannel(channel, response, client.getNickname());
	for (std::vector<Client>::iterator it = this->_clients.begin(); it != this->_clients.end(); ++it)
		this->sendUserList(*it, channel);
}

void Server::joinAsNormal(std::string channelName, Client &client)
{
	std::map<std::string, Channel>::iterator itChannel = this->_channelsM.find(channelName);
	client.getJoinedChannels()[channelName] = &itChannel->second;
	itChannel->second.getClientsM()[client.getNickname()] = &client;

	std::string response = JOIN(client, channelName);
	this->sendResponse(client, response);
	this->sendMsgToAllInChannel(itChannel->second, response, client.getNickname());
	response = R_TOPIC(client, channelName, itChannel->second.getTopic());
	this->sendResponse(client, response);
	for (std::vector<Client>::iterator itClient = this->_clients.begin(); itClient != this->_clients.end(); ++itClient)
		this->sendUserList(*itClient, itChannel->second);
	if (this->_bot_on && (client.getNickname() != this->_bot->getNickname()))
	{
		response = "PRIVMSG " + channelName + " :Welcome " + client.getNickname() + " to channel " + channelName + "!\r\n";
		this->sendResponse(*this->_bot, response);
	}
}


void Server::join(std::vector<std::string> reqVec, Client &client)
{
	if (!client.getNickRegistered())
	{
		std::cout << YELLOW << "Client not registered!" << RESET << std::endl;
		return;
	}
	if (checkJoin(reqVec, client))
	{
		bool passGiven = false;
		std::vector<std::string> channelsToJoin;
		std::vector<std::string> passwords;
		std::string response;
 		createLst(reqVec[1], channelsToJoin);
		if (reqVec.size() >= 3) {
			createLst(reqVec[2], passwords);
			passGiven = true;
		}
		for (unsigned int i = 0; i < channelsToJoin.size(); ++i) {
			if (client.getJoinedChannels().find(channelsToJoin[i]) != client.getJoinedChannels().end())
				continue ;
			if (channelsToJoin[i][0] != '#') {
				response = E_NOSUCHCHANNEL(client, channelsToJoin[i]);
				this->sendResponse(client, response);
			}
			else if (this->_channelsM.find(channelsToJoin[i]) == this->_channelsM.end())
				this->joinAsOperator(channelsToJoin[i], client);
			else {
				if (this->_channelsM[channelsToJoin[i]].getModes()['l'] && this->_channelsM[channelsToJoin[i]].getClientsM().size() >= this->_channelsM[channelsToJoin[i]].getLimit())
					return this->sendResponse(client, E_CANNOTJOINCHAN(client, channelsToJoin[i]));
				if (this->_channelsM[channelsToJoin[i]].getModes()['i']) {
					response = E_INVITEONLYCHAN(client, channelsToJoin[i]);
					this->sendResponse(client, response);
				}
				else if ((!passGiven || i > passwords.size() - 1) && !this->_channelsM[channelsToJoin[i]].getModes()['k'])
					this->joinAsNormal(channelsToJoin[i], client);
				else if (passGiven && i < passwords.size() && this->checkPassword(channelsToJoin[i], passwords[i]))
					this->joinAsNormal(channelsToJoin[i], client);
				else {
					response = E_BADCHANNELKEY(client, channelsToJoin[i]);
					this->sendResponse(client, response);
				}
			}
		}
	}
}
