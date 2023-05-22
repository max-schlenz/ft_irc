# include "irc.hpp"
# include "definitions.hpp"

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

bool checkNick(std::vector<std::string> reqVec, Client& client)
{
	std::string name = reqVec[1];
	std::string clientIp = client.getHostname() + " ";
	std::string currentNick = client.getNickname() + " ";
	std::string err_msg;
	if (reqVec.size() <= 1) { // there is no need fot this if as the irssi client already handles that case
		err_msg = ERR_NONICK + clientIp + ":No nickname given\r\n";
		send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		return false;
	}
	else if (!checkChars(name) || reqVec.size() > 2) {
		err_msg =  ERR_INVALIDNICK + clientIp + currentNick + ":Erroneus nickname\r\n";
		send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		return false;
	}
	return true;
}

bool checkUser(std::vector<std::string> reqVec, Client& client)
{
	std::string clientIp = client.getHostname() + " ";
	std::string err_msg;
	if (client.getUsername().size() <= 0) {
		err_msg = ERR_ALREADYREGISTERED + ":You may not reregister\r\n";
		send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		return false;
	} else {
		if (reqVec.size() < 5) {
			err_msg = ERR_NEEDMOREPARAMS + clientIp + reqVec[0] + " :Not enough parameters\r\n";
			send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
			return false;
		}
	}
	return true;
}

bool checkPart(std::vector<std::string> reqVec, Client& client, std::vector<Channel> channels)
{
	std::string clientIp = client.getHostname() + " ";
	std::string err_msg;
	if (reqVec.size() < 2) {
		err_msg = ERR_NEEDMOREPARAMS + clientIp + reqVec[0] + " :Not enough parameters\r\n";
		send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		return false;
	}
	std::string channelToPart = reqVec[1];
	if (!channelExists(channelToPart, channels)) {
		err_msg = ERR_NOSUCHCHANNEL + channelToPart + " :No such channel\r\n";
		send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		return false;
	}
	if (client.getJoinedChannels().find(channelToPart) == client.getJoinedChannels().end()) {
		err_msg = ERR_NOTONCHANNEL + clientIp + channelToPart + " :You're not on that channel\r\n";
		send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		return false;
	}
	return true;
}

static bool nickExists(std::string nickname, std::vector<Client> clients)
{
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
		if (it->getNickname() == nickname)
			return true;
	}
	std::cout << "nick doesnt exits0" << std::endl;
	return false;
}

static bool nickExists(std::string nickname, std::vector<Client*> clients)
{
	std::cout << "nick doesnt exits1" << std::endl;
	for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
		if ((*it)->getNickname() == nickname)
			return true;
	}
	return false;
}

static bool userOnChannel(std::string nickname, std::string channelname, std::vector<Channel> channels)
{
	return true ;
	// for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); ++it) {
	// 	if (it->getName() == channelname) {
	// 		if (nickExists(nickname, it->getClients())) {
	// 			std::cout << "working :)" << std::endl;
	// 			return true;
	// 		}
	// 		std::cout << "not working" << std::endl;
	// 		return false;
	// 	}
	// }
	return false;
}

bool checkInvite(std::vector<std::string> reqVec, Client& client, std::vector<Channel> channels, Server& server) {
	std::string clientIp = client.getHostname() + " ";
	std::string err_msg;
	if (reqVec.size() < 3) {
		err_msg = ERR_NEEDMOREPARAMS + clientIp + reqVec[0] + " :Not enough parameters\r\n";
		send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		return false;
	}
	std::string channelToInvite = reqVec[2];
	if (!channelExists(channelToInvite, channels)) {
		err_msg = ERR_NOSUCHCHANNEL + clientIp +  channelToInvite + ":No such channel\r\n";
		send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		return false;
	}
	if (!nickExists(reqVec[1], server.getClients())) {
		err_msg = ERR_NOSUCHNICK + clientIp +  reqVec[1] + ":No such nick/channel\r\n";
		send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		return false;
	}
	if (client.getJoinedChannels().find(channelToInvite) == client.getJoinedChannels().end()) {
		err_msg = ERR_NOTONCHANNEL + clientIp + channelToInvite + " :You're not on that channel\r\n";
		send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		return false;
	}
	if (userOnChannel(reqVec[1], channelToInvite, channels)) { ///not working why????
		err_msg = ERR_USERONCHANNEL + clientIp + channelToInvite + " :is already on channel";
		std::cout << err_msg << std::endl;
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

bool checkTopic(std::vector<std::string> reqVec, Client& client)
{
	return true;
	// std::string clientIp = client.getHostname() + " ";
	// std::string err_msg;
	// if (reqVec.size() < 3) {
	// 	err_msg = ERR_NEEDMOREPARAMS + clientIp + reqVec[0] + " :Not enough parameters\r\n";
	// 	send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
	// 	return false;
	// }
	// std::string channel = reqVec[2];
	// if (!channelExists(channel, channels)) {
	// 	err_msg = ERR_NOSUCHCHANNEL + clientIp +  channel + ":No such channel\r\n";
	// 	send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
	// 	return false;
	// }
	// if (client.getJoinedChannels().find(channel) == client.getJoinedChannels().end()) {
	// 	err_msg = ERR_NOTONCHANNEL + clientIp + channel + " :You're not on that channel\r\n";
	// 	send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
	// 	return false;
	// }
	// // if (/*protected topic mode && not operator*/) {
	// // 	err_msg = ERR_CHANOPRIVSNEEDED + clientIp + channel + " :You're not channel operator\r\n";
	// // 	send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
	// // 	return false;
	// // }

}