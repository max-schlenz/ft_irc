# include "irc.hpp"
# include "definitions.hpp"

static void createLst(std::string req, std::vector<std::string>& lst)
{
	std::string buffer;
	std::istringstream iss(req);

	while (getline(iss, buffer, ',')){
		lst.push_back(buffer);
		buffer.clear();
	}
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

// bool checkChannelJoin(std::string channelName) {

// }

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
	
	// std::string channelToJoin = reqVec[1];
	// if (this->_channelsM.find(channelToJoin) == this->_channelsM.end()) {
	// 	std::string err_msg = msg_2(this->_hostname, ERR_NOSUCHCHANNEL, clientIp, channelToJoin, "No such channel");
	// 	send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
	// 	return false;
	// }
	// if (reqVec.size() == 2) {
	// 	createLst(reqVec[1], channelsToJoin);
	// 	for (int i = 0; i < channelsToJoin.size(); ++i) {
	// 		// if (this->_channelsM.find(channelToJoin) == this->_channelsM.end()) {
	// 		// 	joinAsOp();
	// 		// } else {
	// 		// 	join();
	// 		// }
	// 		if (/*invite only channe*/) {
	// 			err_msg = msg_2(this->_hostname, ERR_INVITEONLYCHAN, clientIp, reqVec[0], "Cannot join channel (+i)");
	// 			send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
	// 			return false;
	// 		}
	// 	}
	// }
	return true;
}
