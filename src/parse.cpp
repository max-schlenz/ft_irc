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
	std::string clientIp = client.getIpStr() + " ";
	std::string currentNick = client.getNickname() + " ";
	std::string err_msg;
	if (reqVec.size() <= 1) { // there is no need fot this if as the irssi client already handles that case
		err_msg = ERR_NONICK + clientIp + ":No nickname given\r\n";
		send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		return false;
	}
	else if (!checkChars(name)) {
		err_msg =  ERR_INVALIDNICK + clientIp + currentNick + ":Erroneus nickname\r\n";
		send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		return false;
	}
	return true;
}

bool checkUser(std::vector<std::string> reqVec, Client& client)
{
	std::string clientIp = client.getIpStr() + " ";
	std::string err_msg;
	if (client.getUsername().size() <= 0) {
		err_msg = ERR_ALREADYREGISTERED + ":You may not reregister\r\n";
		send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		return false;
	} else {
		if (reqVec.size() < 1) {
			err_msg = ERR_NEEDMOREPARAMS + clientIp + reqVec[0] + " :Not enough parameters";
			send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
			return false;
		}
	}
	return true;
}

bool checkPart(std::vector<std::string> reqVec, Client& client, std::vector<Channel> channels)
{
	std::string clientIp = client.getIpStr() + " ";
	std::string err_msg;
	if (reqVec.size() < 2) {
		err_msg = ERR_NEEDMOREPARAMS + clientIp + reqVec[0] + " :Not enough parameters";
		send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		return false;
	}
	std::string channelToPart = reqVec[2].replace(0, 1, "#");
	if (!channelExists(channelToPart, channels)) {
		err_msg = ERR_NOSUCHCHANNEL + channelToPart + " :No such channel";
		send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		return false;
	}
	if (!channelExists(channelToPart, client.getJoinedChannels())) {
		err_msg = ERR_NOTONCHANNEL + clientIp + channelToPart + " :You're not on that channel";
		send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		return false;
	}
	return true;
}


bool checkInvite(std::vector<std::string> reqVec, Client& client, std::vector<Channel> channels) {
	std::string clientIp = client.getIpStr() + " ";
	std::string err_msg;
	if (reqVec.size() < 2) {
		err_msg = ERR_NEEDMOREPARAMS + clientIp + reqVec[0] + " :Not enough parameters";
		send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		return false;
	}
	std::string channelToPart = reqVec[2].replace(0, 1, "#");
	if (!channelExists(channelToPart, channels)) {
		err_msg = ERR_NOSUCHCHANNEL + channelToPart + " :No such channel";
		send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		return false;
	}
	if (/*member not on channel*/) {
		err_msg = ERR_NOTONCHANNEL + clientIp + channelToPart + " :You're not on that channel";
		send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		return false;
	}
	if (/*invite only channel & client not operator*/) {
		err_msg = ERR_CHANOPRIVSNEEDED + clientIp + channelToPart + " :You're not channel operator";
		send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		return false;
	}
	if (/*user to be invited is already on channel*/) {
		err_msg = ERR_USERONCHANNEL + clientIp  + client.getNickname() + " " + channelToPart + " :is already on channel";
		send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		return false;
	}
	return true;
}

bool checkKick(std::vector<std::string> reqVec, Client& client, std::vector<Channel> channels) {

}