# include "irc.hpp"
# include "definitions.hpp"
# include "Client.hpp"

static bool checkChars(std::string name)
{
	std::string notAllowed = ",*?!@.";
	std::string notStart = ":$#&";
	for (int i = 0; i < notAllowed.size(); ++i) {
		if (name.find(notAllowed[i]) != std::string::npos)
			return false;
	}
	if (notStart.find(name[0]) != std::string::npos || isdigit(name[0]))
		return false;
	return true;
}

bool Server::checkNick(std::vector<std::string> reqVec, Client& client)
{
	std::string response;
	if (reqVec.size() <= 1) { // there is no need fot this if as the irssi client already handles that case
		response = E_NONICKNAMEGIVEN(client);
		this->sendResponse(client, response);
		return false;
	}
	if (!checkChars(reqVec[1]) || reqVec.size() > 2) {
		response = E_ERRONEUSNICKNAME(client, reqVec[1]);
		this->sendResponse(client, response);
	std::string name = reqVec[1];
	if (client.getNickname() == name)
		return false;
	}
	if (this->_clientsM.find(name) != this->_clientsM.end()) {
		err_msg = msg_2(this->_hostname, ERR_NICKINUSE, clientIp, name, "Nickname is already in use");
		send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		return false;
	}
	return true;
}

bool Server::checkUser(std::vector<std::string> reqVec, Client& client)
{
	std::string response;
	if (client.getUsername().size() <= 0) {
		response = E_ALREADYREGISTERED(client);
		this->sendResponse(client, response);
		return false;
	} else {
		if (reqVec.size() < 5) {
			response = E_NEEDMOREPARAMS(client, reqVec[0]);
			this->sendResponse(client, response);
			return false;
		}
	}
	return true;
}

bool Server::checkPart(std::vector<std::string> reqVec, Client& client)
{
	std::string	response;

	if (reqVec.size() < 2) {
		response = E_NEEDMOREPARAMS(client, reqVec[0]);
		this->sendResponse(client, response);
		return false;
	}
	std::string channelToPart = reqVec[1];
	if (this->_channelsM.find(channelToPart) == this->_channelsM.end()) {
		response = E_NOSUCHCHANNEL(client, channelToPart);
		this->sendResponse(client, response);
		return false;
	}
	if (client.getJoinedChannels().find(channelToPart) == client.getJoinedChannels().end()) {
		response = E_NOTONCHANNEL(client, channelToPart);
		this->sendResponse(client, response);
		return false;
	}
	return true;
}

bool Server::checkInvite(std::vector<std::string> reqVec, Client& client) {
	std::string response;
	if (reqVec.size() < 3) {
		response = E_NEEDMOREPARAMS(client, reqVec[0]);
		this->sendResponse(client, response);
		return false;
	}
	std::string channelToInvite = reqVec[2];
	if (this->_channelsM.find(channelToInvite) == this->_channelsM.end()) {
		response = E_NOSUCHCHANNEL(client, channelToInvite);
		this->sendResponse(client, response);
		return false;
	}
	if (this->_clientsM.find(reqVec[1]) == this->_clientsM.end()) {
		response = E_NOSUCHNICK(client, reqVec[1]);
		this->sendResponse(client, response);
		return false;
	}
	if (client.getJoinedChannels().find(channelToInvite) == client.getJoinedChannels().end()) {
		response = E_NOTONCHANNEL(client, channelToInvite);
		this->sendResponse(client, response);
		return false;
	}
	if (this->_channelsM[channelToInvite].getClientsM().find(reqVec[1]) != this->_channelsM[channelToInvite].getClientsM().end()) { ///not working why????
		std::cout << "catch user on channel error" << std::endl;
		std::cout << "clients size: " << this->_clientsM.size() << std::endl;
		for(std::map<std::string, Client*>::iterator it = this->_channelsM[channelToInvite].getClientsM().begin();
			it != this->_channelsM[channelToInvite].getClientsM().end(); ++it)
		{
			std::cout << it->first << "\n";
		}
		response = E_USERONCHANNEL(client, reqVec[1], channelToInvite);
		this->sendResponse(client, response);
		return false;
	}
	// if (/*invite only channel & client not operator*/) {
	// 	err_msg = ERR_CHANOPRIVSNEEDED + clientIp + channelToPart + " :You're not channel operator";
	// 	send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
	// 	return false;
	// }
	return true;
}

bool Server::checkTopic(std::vector<std::string> reqVec, Client& client)
{
	std::string clientIp = client.getHostname();
	std::string err_msg;
	std::string response;
	if (reqVec.size() < 3) {
		E_NEEDMOREPARAMS(client, reqVec[0]);
		send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		return false;
	}
	std::string channelName = reqVec[2];
	if (this->_channelsM.find(channelName) == this->_channelsM.end()) {
		response = E_NOSUCHCHANNEL(client, channelName);
		this->sendResponse(client, response);
		return false;
	}
	if (client.getJoinedChannels().find(channelName) == client.getJoinedChannels().end()) {
		response = E_NOTONCHANNEL(client, channelName);
		this->sendResponse(client, response);
		return false;
	}
	// if (/*protected topic mode && not operator*/) {
	// 	err_msg = msg_2(this->_hostname, ERR_CHANOPRIVSNEEDED, clientIp, channel, "You're not channel operator");
	// 	send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
	// 	return false;
	// }
	return true;

}

bool Server::checkJoin(std::vector<std::string> reqVec, Client& client)
{
	std::string clientIp = client.getHostname();
	std::string err_msg;

	if (reqVec.size() < 2) {
		err_msg = E_NEEDMOREPARAMS(client, reqVec[0]);
		send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		return false;
	}
	return true;
}

static std::string getUserModes(Client& client)
{
	std::string modes = "+";
	if (client.getModeI())
		modes += "i";
	if (client.getModeO())
		modes += "o";
	return modes;
}

static std::string getChannelModes(Channel& channel)
{
	std::string modes = "+";
	if (channel.getModes()['k'])
		modes += "k";
	if (channel.getModes()['i'])
		modes += "i";
	if (channel.getModes()['t'])
		modes += "t";
	return modes;
}

bool Server::checkUserMode(std::vector<std::string> reqVec, Client& client)
{
	std::string clientIp = client.getHostname();
	std::string err_msg;
	std::string response;
	if (reqVec.size() < 2) {
		err_msg = E_NEEDMOREPARAMS(client, reqVec[0]);
		this->sendResponse(client, response);
		return false;
	}
	std::string nickname = reqVec[1];
	if (this->_clientsM.find(nickname) == this->_clientsM.end()) {
		response = E_NOSUCHNICK(client, reqVec[1]);
		this->sendResponse(client, response);
		return false;
	}
	if (client.getNickname() != reqVec[1]) {
		response = E_USERSDONTMATCH(client);
		this->sendResponse(client, response);
		return false;
	}
	if (reqVec.size() < 3) {
		std::string modes = getUserModes(client);
		err_msg = msg_1(this->_hostname, RPL_UMODEIS, clientIp, modes);
		send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		return false;
	}
	return true;
}

bool Server::checkChannelMode(std::vector<std::string> reqVec, Client& client)
{
	std::string clientIp = client.getHostname();
	std::string err_msg;
	if (reqVec.size() < 2) {
		err_msg = msg_2(this->_hostname, ERR_NEEDMOREPARAMS, clientIp, reqVec[0], "Not enough parameters");
		send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		return false;
	}
	std::string channelName = reqVec[1];
	if (this->_channelsM.find(channelName) == this->_channelsM.end()) {
		std::string err_msg = msg_2(this->_hostname, ERR_NOSUCHCHANNEL, clientIp, channelName, "No such channel");
		send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		return false;
	}
	if (reqVec.size() < 3) {
	// "<client> <channel> <modestring> <mode arguments>..."
		std::string modes = getChannelModes(this->_channelsM[channelName]);
		std::cout << "channel modes: " << modes << std::endl;
		// err_msg = msg_2(this->_hostname, RPL_CHANNELMODEIS, clientIp, modes, channelName);
		err_msg = msg_2(this->_hostname, RPL_CHANNELMODEIS, clientIp, modes + channelName, modes);
		send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		return false;
	}
	return true;
}