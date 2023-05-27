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
			response = E_UMODEUNKNOWNFLAG(client, modes[i]);
			this->sendResponse(client, response);
		} else {
			if (operation == "-" && !client.getModeI() || operation == "+" && client.getModeI())
				continue;
			client.setModeI(true);
			if (operation == "-")
				client.setModeI(false);
			// err_msg = msg_2(this->_hostname, "MODE", client.getNickname(), operation + modes[i], "changing mode");
			response = USERMODE(client, operation + modes[i]);
			// send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
			this->sendResponse(client, response);
		}
	}
}

void Server::handleModeK(std::vector<std::string> reqVec, Client &client, int i, int args_counter, std::string operation)
{
	std::string channelName = reqVec[1];
	std::vector<std::string> args;
	std::map<char, bool> channelModes = this->_channelsM[channelName].getModes();
	std::string response;
	bool argsGiven = false;
	if (reqVec.size() >= 4) {
		createLst(reqVec[3], args);
		argsGiven = true;
	}
	if ((!argsGiven || args_counter >= args.size())) {
		std::cout << "in herer\n" << std::endl;
		response = E_INVALIDMODEPARAM(client, channelName, "k", "You must specify a parameter for the key mode. Syntax: <key>");
		this->sendResponse(client, response);
	}
	if (argsGiven && args_counter < args.size()) {
		std::cout << "in herer\n" << std::endl;
		if (!channelModes['k'] || this->checkPassword(channelName, args[args_counter], client)) {
			response = CHANNELMODEARGS(client, channelName, operation + "k", args[args_counter]);
			this->_channelsM[channelName].setModeTrue('k');
			this->_channelsM[channelName].setPassword(args[args_counter]);
			if (operation == "-") {
				this->_channelsM[channelName].setModeFalse('k');
				this->_channelsM[channelName].setPassword("");
			}
		} else {
			// response = E_INVALIDKEY(client, channelName);
			std::cout << "invalid key" << std::endl;
			response = ERR_KEYSET(client, channelName);
		}
		this->sendResponse(client, response);
	}
}

void Server::channelModeLoop(std::vector<std::string> reqVec, Client &client)
{
	
	std::string channelName = reqVec[1];
	std::string modes = reqVec[2];
	Channel& channel = this->_channelsM[channelName];
	std::map<char, bool> channelModes = this->_channelsM[channelName].getModes();
	std::string clientIp = client.getHostname();
	std::string response;
	std::vector<std::string> args;
	int args_counter = 0;
	std::string operation = "+";
	bool argsGiven = false;
	if (reqVec.size() >= 4) {
		createLst(reqVec[3], args);
		argsGiven = true;
	}
	int i = 1;
	std::cout << " mode in here\n" << std::endl;
	if (modes[0] != '-') {
		operation = "-";
	}
	for (int i = 0; i < modes.size(); ++i) {
		std::cout << "chanel modes k:" << channelModes['k'] << std::endl;
		std::cout << modes[i] << argsGiven << (args_counter < args.size()) << !channelModes['k'] << std::endl;
		if (modes[i] == '-') {
			operation = "-";
			continue;
		}
		else if (modes[i] == '+') {
			operation = "+";
			continue;
		}
		else if (!validChannelMode(channelModes, modes[i])) {
			std::string mode(1, modes[i]);
			response = E_UMODEUNKNOWNFLAG(client, modes[i]);
			// err_msg = msg_2(this->_hostname, ERR_UMODEUNKNOWNFLAG, client.getNickname(), mode, "is not a recognised channel mode");
			this->sendResponse(client, response);
		} //else if (channel.getOperators().find(client.getNickname()) == channel.getOperators().end()) {
		// 	response = E_CHANOPRIVSNEEDED(client, channelName);
		// 	// err_msg = msg_2(this->_hostname, ERR_CHANOPRIVSNEEDED, clientIp, channelName, "You're not channel operator");
		// 	this->sendResponse(client, response);
		// }
		else if (modes[i] == 'k')
			this->handleModeK(reqVec, client, i, args_counter, operation);
		else if (modes[i] == 'o') {
			response = E_INVALIDMODEPARAM(client, channelName, "o", "You must specify a parameter for the operator mode. Syntax: <nick>");
			// err_msg = msg_4(this->_hostname, ERR_INVALIDMODEPARAM, clientIp, channelName, "o", "*", "You must specify a parameter for the operator mode. Syntax: <nick>");
			this->sendResponse(client, response);
		}
		else if (modeAlreadyOper(operation, modes[i], channelModes)) {
			++args_counter;
			continue;
		}
		else {
			std::cout << "in here wtf" << std::endl;
			if (operation == "-")
				this->_channelsM[channelName].setModeFalse(modes[i]);
			else
				this->_channelsM[channelName].setModeTrue(modes[i]);
			response = CHANNELMODE(client, channelName, operation + modes[i]);
			// err_msg = msg_2(this->_hostname, "MODE", client.getNickname(), operation + modes[i], "changing mode");
			// send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
			this->sendResponse(client, response);
		}
		++args_counter;
	}
}

void Server::channelMode(std::vector<std::string> reqVec, Client &client)
{
	channelModeLoop(reqVec, client);
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
