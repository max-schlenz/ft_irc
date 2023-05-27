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

void Server::mode(std::vector<std::string> reqVec, Client &client)
{
	if (isUserMode(reqVec[1]) && this->checkUserMode(reqVec, client)) {
		this->userMode(reqVec, client);
		std::cout << client.getNickname() << GRAY << " mode" << std::endl;
	}
	// else if (this->checkChannelMode(reqVec, client)) {
		
	// }
}
