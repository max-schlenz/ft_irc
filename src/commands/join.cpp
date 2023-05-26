#include "Server.hpp"
# include "irc.hpp"
# include "definitions.hpp"

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

static void createLst(std::string req, std::vector<std::string>& lst)
{
	std::string buffer;
	std::istringstream iss(req);

	while (getline(iss, buffer, ',')){
		lst.push_back(buffer);
		buffer.clear();
	}
}

bool Server::checkPassword(std::string channelName, std::string password, Client& client) {
	if (this->_channelsM[channelName].getModes()['k']) {
		std::cout << this->_channelsM[channelName].getPassword() << "\t" << password << std::endl;
		if (this->_channelsM[channelName].getPassword() != password) {
			std::string err_msg = msg_2(this->_hostname, ERR_BADCHANNELKEY, client.getHostname(), channelName, "Cannot join channel (+k)");
			send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
			return false;
		}
	}
	return true;
}

void Server::joinAsOperator(std::string channelName, Client &client)
{
	Channel channel(channelName);
	this->_channelsM[channelName] = channel;
	this->_channelsM[channelName].getClientsM()[client.getNickname()] = &client;
	client.getJoinedChannels()[channelName] = &this->_channelsM[channelName];

	std::string response = ":" + client.getNickname() + "!~" + client.getUsername() + "@localhost JOIN " + channelName + "\r\n";
	this->sendResponse(client, response);
	this->sendMsgToAllInChannel(channel, response, client); //brauch man das?
	for (std::vector<Client>::iterator it = this->_clients.begin(); it != this->_clients.end(); ++it)
		this->sendUserList(*it, channel);
}

void Server::joinAsNormal(std::string channelName, Client &client)
{
	std::map<std::string, Channel>::iterator itChannel = this->_channelsM.find(channelName);
	client.getJoinedChannels()[channelName] = &itChannel->second;
	itChannel->second.getClientsM()[client.getNickname()] = &client;

	std::string response = ":" + client.getNickname() + "!~" + client.getUsername() + "@localhost JOIN " + channelName + "\r\n";
	this->sendResponse(client, response);

	this->sendMsgToAllInChannel(itChannel->second, response, client);
	// this->sendMsgToAll(client, response);
	for (std::vector<Client>::iterator itClient = this->_clients.begin(); itClient != this->_clients.end(); ++itClient)
		this->sendUserList(*itClient, itChannel->second);

	return;
}


//: NickName!UserName@host JOIN #channelname
void Server::join(std::vector<std::string> reqVec, Client &client)
{
	if (checkJoin(reqVec, client))
	{
		if (client.getJoinedChannels().find(reqVec[1]) != client.getJoinedChannels().end())
			return;
		std::vector<std::string> channelsToJoin;
		createLst(reqVec[1], channelsToJoin);
		if (reqVec.size() == 2) {
			for (int i = 0; i < channelsToJoin.size(); ++i) {
				if (this->_channelsM.find(channelsToJoin[i]) == this->_channelsM.end())
					this->joinAsOperator(channelsToJoin[i], client);
				else {
					if (this->checkPassword(channelsToJoin[i], "", client)) {
						this->joinAsNormal(channelsToJoin[i], client);
					}
				}
			}
		}
		if (reqVec.size() >= 3) {
			std::vector<std::string> passwords;
			createLst(reqVec[2], passwords);
			for (int i = 0; i < channelsToJoin.size(); ++i) {
				for (int j = 0; j < passwords.size(); ++j) {
					if (this->_channelsM.find(channelsToJoin[i]) == this->_channelsM.end())
						this->joinAsOperator(channelsToJoin[i], client);
					else {
						if (this->checkPassword(channelsToJoin[i], passwords[j], client))
							this->joinAsNormal(channelsToJoin[i], client);
					}
				}
			}
		}
	}
}
