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
	std::string name = reqVec[1];
	std::string clientIp = client.getHostname();
	std::string currentNick = client.getNickname();
	std::string err_msg;
	if (reqVec.size() <= 1) { // there is no need fot this if as the irssi client already handles that case
		err_msg = msg_1(this->_hostname, ERR_NONICK, clientIp, "No nickname given");
		send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		return false;
	}
	if (!checkChars(name) || reqVec.size() > 2) {
		err_msg = msg_2(this->_hostname, ERR_INVALIDNICK, clientIp, reqVec[1], "Erroneus nickname");
		send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		return false;
	}
	return true;
}

bool Server::checkUser(std::vector<std::string> reqVec, Client& client)
{
	std::string clientIp = client.getHostname();
	std::string err_msg;
	if (client.getUsername().size() <= 0) {
		err_msg = msg_0(this->_hostname, ERR_ALREADYREGISTERED, "You may not reregister");
		send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		return false;
	} else {
		if (reqVec.size() < 5) {
			err_msg = msg_2(this->_hostname, ERR_NEEDMOREPARAMS, clientIp, reqVec[0], "Not enough parameters");
			send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
			return false;
		}
	}
	return true;
}

bool Server::checkPart(std::vector<std::string> reqVec, Client& client)
{
	std::string clientIp = client.getHostname() + " ";
	std::string err_msg;
	if (reqVec.size() < 2) {
		err_msg = msg_2(this->_hostname, ERR_NEEDMOREPARAMS, clientIp, reqVec[0], "Not enough parameters");
		send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		return false;
	}
	std::string channelToPart = reqVec[1];
	if (this->_channelsM.find(channelToPart) == this->_channelsM.end()) {
		std::string err_msg = msg_2(this->_hostname, ERR_NOSUCHCHANNEL, clientIp, channelToPart, "No such channel");
		send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		return false;
	}
	if (client.getJoinedChannels().find(channelToPart) == client.getJoinedChannels().end()) {
		err_msg = msg_2(this->_hostname, ERR_NOTONCHANNEL, clientIp, channelToPart, "You're not on that channel");
		send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		return false;
	}
	return true;
}

bool Server::checkInvite(std::vector<std::string> reqVec, Client& client) {
	std::string clientIp = client.getHostname();
	std::string err_msg;
	if (reqVec.size() < 3) {
		err_msg = msg_2(this->_hostname, ERR_NEEDMOREPARAMS, clientIp, reqVec[0], "Not enough parameters");
		send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		return false;
	}
	std::string channelToInvite = reqVec[2];
	if (this->_channelsM.find(channelToInvite) == this->_channelsM.end()) {
		err_msg = msg_2(this->_hostname, ERR_NOSUCHCHANNEL, clientIp, channelToInvite, "No such channel");
		send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		return false;
	}
	if (this->_clientsM.find(reqVec[1]) == this->_clientsM.end()) {
		err_msg = msg_2(this->_hostname, ERR_NOSUCHNICK, clientIp, reqVec[1], "No such nick/channel");
		send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		return false;
	}
	if (client.getJoinedChannels().find(channelToInvite) == client.getJoinedChannels().end()) {
		err_msg = msg_2(this->_hostname, ERR_NOTONCHANNEL, clientIp, channelToInvite, "You're not on that channel");
		send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
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
		err_msg = msg_3(this->_hostname, ERR_USERONCHANNEL, clientIp, reqVec[1], channelToInvite, "is already on channel");
		send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
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
	std::string clientIp = client.getHostname() + " ";
	std::string err_msg;
	if (reqVec.size() < 3) {
		err_msg = msg_2(this->_hostname, ERR_NEEDMOREPARAMS, clientIp, reqVec[0], "Not enough parameters");
		send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		return false;
	}
	std::string channelName = reqVec[2];
	if (this->_channelsM.find(channelName) == this->_channelsM.end()) {
		std::cout << "lol" << std::endl;
		std::string err_msg = msg_2(this->_hostname, ERR_NOSUCHCHANNEL, clientIp, channelName, "No such channel");
		send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		return false;
	}
	if (client.getJoinedChannels().find(channelName) == client.getJoinedChannels().end()) {
		err_msg = msg_2(this->_hostname, ERR_NOTONCHANNEL, clientIp, channelName, "You're not on that channel");
		send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		return false;
	}
	// if (/*protected topic mode && not operator*/) {
	// 	err_msg = msg_2(this->_hostname, ERR_CHANOPRIVSNEEDED, clientIp, channel, "You're not channel operator");
	// 	send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
	// 	return false;
	// }
	return true;

}