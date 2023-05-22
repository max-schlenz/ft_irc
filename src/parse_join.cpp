# include "irc.hpp"
# include "definitions.hpp"

std::vector<std::string> getLst(std::string req)
{
	std::string buffer;
	std::vector<std::string> lst;
	std::istringstream iss(req);

	while (getline(iss, buffer, ',')){
		lst.push_back(buffer);
		buffer.clear();
	}
	return lst;
}

bool channelExists(std::string channelName, std::vector<Channel> channels) {
	std::cout << "name: " << channelName << std::endl;
	for (int i = 0; i < channels.size(); ++i){
		std::cout << "real name: " << channels[i].getName() << std::endl;
		if (channels[i].getName() == channelName)
			return true;
	}
	return false;
}

// absolutely not working yet
bool Server::checkJoin(std::vector<std::string> reqVec, Client& client)
{
	std::string clientIp = client.getHostname();
	std::string err_msg;
	std::vector<std::string> channelsToJoin;
	std::vector<std::string> passwords;

	if (reqVec.size() < 2) {
		err_msg = msg_2(this->_hostname, ERR_NEEDMOREPARAMS, clientIp, reqVec[0], "Not enough parameters");
		send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		return false;
	}
	std::string channelToJoin = reqVec[1];
	if (this->_channelsM.find(channelToJoin) == this->_channelsM.end()) {
		std::cout << "lol" << std::endl;
		std::string err_msg = msg_2(this->_hostname, ERR_NOSUCHCHANNEL, clientIp, channelToJoin, "No such channel");
		send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		return false;
	}
	// if (!channelExists(reqVec[1], channels)) {
	// 	std::cout << "Channel doesnt exist yet, creaate operaator" << std::endl; // logic ist still missing here
	// } else if (reqVec.size() == 2) {
	// 	channelsToJoin = getLst(reqVec[1]);
	// 	// for (int i = 0; i < channels.size(); ++i) {
	// 	// 	if (!channelExists(channels[i].getName(), channels))
	// 	// 			//handle what should happen in channel doesnt exist?
	// 	// 			return false
	// 	// }
	// } else if (reqVec.size() == 3) {
	// 	channelsToJoin = getLst(reqVec[1]);
	// 	passwords = getLst(reqVec[2]);
	// 	for (int i = 0; i <= channels.size(); ++i) {
	// 		// if (!channelExists(channels[i].getName(), channels)
	// 		// 	//handle channel not yet existing
	// 		// 	return false
	// 		if (channels[i].getPassword() != passwords[i]) {
	// 			err_msg = ERR_BADCHANNELKEY + clientIp + ":Cannot join channel (+k)";
	// 			send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
	// 			return false;
	// 		}
	// 		///otherwise join channel? or coomplete knockoff
	// 	}
	// } else {
	// 	std::cout << "Join too many arguments" << std::endl;
	// }
	return true;
}
