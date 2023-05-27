# include "Server.hpp"
# include "irc.hpp"

static bool isUserMode(std::string channelName)
{
	if (channelName[0] != '#')
		return true;
	return false;
}

static bool validUserMode(char mode) {
	std::string modes = "i";

	if (modes.find(mode) == std::string::npos)
		return false;
	return true;
}

static bool validChannelMode(std::map<char, bool> modes, char mode) {
	if (modes.find(mode) == modes.end() && mode != 'o')
		return false;
	return true;
}

static bool modeAlreadyOper(std::string operation, char mode, std::map<char, bool> modes)
{
	if (operation == "-" && !modes[mode] || operation == "+" && modes[mode])
		return true;
	return false;
}

void Server::userMode(std::vector<std::string> reqVec, Client &client)
{
	std::string modes = reqVec[2];
	std::string clientIp = client.getHostname();
	std::string err_msg;
	std::string response;
	std::string operation = "+";
	int i = 1;
	std::cout << " mode in here\n" << std::endl;
	if (modes[0] != '-') {
		operation = "-";
	}
	for (int i = 0; i < modes.size(); ++i) {
		if (modes[i] == '-')
			operation = "-";
		else if (modes[i] == '+')
			operation = "+";
		else if (!validUserMode(modes[i])) {
			response = E_UMODEUNKNOWNFLAG(client);
			this->sendResponse(client, response);
		} else {
			if (operation == "-" && !client.getModeI() || operation == "+" && client.getModeI())
				continue;
			client.setModeI(true);
			if (operation == "-")
				client.setModeI(false);
			err_msg = msg_2(this->_hostname, "MODE", client.getNickname(), operation + modes[i], "changing mode");
			send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		}
	}
}

void Server::channelModeLoop(std::vector<std::string> reqVec, Client &client)
{
	std::string modes = reqVec[2];
	std::string channelName = reqVec[1];
	Channel& channel = this->_channelsM[channelName];
	std::map<char, bool> channelModes = channel.getModes();
	std::string clientIp = client.getHostname();
	std::string err_msg;
	std::vector<std::string> args;
	std::string operation = "+";
	bool argsGiven = false;
	if (reqVec.size() >= 3) {
		createLst(reqVec[2], args);
		argsGiven = true;
	}
	int i = 1;
	std::cout << " mode in here\n" << std::endl;
	if (modes[0] != '-') {
		operation = "-";
	}
	for (int i = 0; i < modes.size(); ++i) {
		if (modes[i] == '-')
			operation = "-";
		else if (modes[i] == '+')
			operation = "+";
		else if (!validChannelMode(channelModes, modes[i])) {
			std::string mode(1, modes[i]);
			err_msg = msg_2(this->_hostname, ERR_UMODEUNKNOWNFLAG, client.getNickname(), mode, "is not a recognised channel mode");
			send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		} else if (channel.getOperators().find(client.getNickname()) == channel.getOperators().end()) {
			err_msg = msg_2(this->_hostname, ERR_CHANOPRIVSNEEDED, clientIp, channelName, "You're not channel operator");
			send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		}
		else if (modes[i] == 'k' && !argsGiven) {
			err_msg = msg_4(this->_hostname, ERR_INVALIDMODEPARAM, clientIp, channelName, "k", "*", "You must specify a parameter for the key mode. Syntax: <key>");
			send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		} else if (modes[i] == 'k' && )
		
		else if (modes[i] == 'o') {
			err_msg = msg_4(this->_hostname, ERR_INVALIDMODEPARAM, clientIp, channelName, "o", "*", "You must specify a parameter for the key mode. Syntax: <nick>");
			send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		}
		else if (modeAlreadyOper(operation, modes[i], channelModes))
			continue;
		else {
			if (operation == "-")
				this->_channelsM[channelName].setModeFalse(modes[i]);
			else
				this->_channelsM[channelName].setModeTrue(modes[i]);
			err_msg = msg_2(this->_hostname, "MODE", client.getNickname(), operation + modes[i], "changing mode");
			send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		}
	}
}

/*void Server::channelModeLoop2(std::vector<std::string> reqVec, Client &client)
{
	std::string modes = reqVec[2];
	std::string channelName = reqVec[1];
	Channel& channel = this->_channelsM[channelName];
	std::map<char, bool> channelModes = channel.getModes();
	std::string clientIp = client.getHostname();
	std::string args;
	std::string err_msg;
	std::string operation = "+";

	int i = 1;
	std::cout << " mode in here\n" << std::endl;
	createLst(reqVec[3], args);
	if (modes[0] != '-') {
		operation = "-";
	}
	for (int i = 0; i < modes.size(); ++i) {
		for (int j = 0; )
		if (modes[i] == '-')
			operation = "-";
		else if (modes[i] == '+')
			operation = "+";
		else if (!validChannelMode(channelModes, modes[i])) {
			std::string mode(1, modes[i]);
			err_msg = msg_2(this->_hostname, ERR_UMODEUNKNOWNFLAG, client.getNickname(), mode, "is not a recognised channel mode");
			send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		} else if (channel.getOperators().find(client.getNickname()) == channel.getOperators().end()) {
			err_msg = msg_2(this->_hostname, ERR_CHANOPRIVSNEEDED, clientIp, channelName, "You're not channel operator");
			send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		}
		else if (modes[i] == 'k') {
			err_msg = msg_4(this->_hostname, ERR_INVALIDMODEPARAM, clientIp, channelName, "k", "*", "You must specify a parameter for the key mode. Syntax: <key>");
			send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		} else if (modes[i] == 'o') {
			err_msg = msg_4(this->_hostname, ERR_INVALIDMODEPARAM, clientIp, channelName, "o", "*", "You must specify a parameter for the key mode. Syntax: <nick>");
			send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		}
		else if (modeAlreadyOper(operation, modes[i], channelModes))
			continue;
		else {
			if (operation == "-")
				this->_channelsM[channelName].setModeFalse(modes[i]);
			else
				this->_channelsM[channelName].setModeTrue(modes[i]);
			err_msg = msg_2(this->_hostname, "MODE", client.getNickname(), operation + modes[i], "changing mode");
			send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		}
	}
}*/

void Server::channelMode(std::vector<std::string> reqVec, Client &client)
{
	if (reqVec.size() < 4) {
		channelModeLoop(reqVec, client);
	}
}

void Server::mode(std::vector<std::string> reqVec, Client &client)
{
	if (isUserMode(reqVec[1]) && this->checkUserMode(reqVec, client)) {
		this->userMode(reqVec, client);
		std::cout << client.getNickname() << GRAY << " mode" << std::endl;
	}
	else if (!isUserMode(reqVec[1]) && this->checkChannelMode(reqVec, client)) {
		channelMode(reqVec, client);
		std::cout << "change channel mode" << std::endl;
	}
}
