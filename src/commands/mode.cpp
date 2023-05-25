#include "Server.hpp"

static bool isUserMode(std::string channelName)
{
	if (channelName[0] != '#')
		return true;
	return false;
}

void Server::mode(std::vector<std::string> reqVec, Client &client)
{
	if (isUserMode(reqVec[1]) && this->checkUserMode(reqVec, client)) {
		// std::string modes = reqVec[2];
		// if (!validUserMode(modes)) {
		// 	err_msg = msg_1(this->_hostname, ERR_UMODEUNKNOWNFLAG, clientIp, "Unknown MODE flag");
		// 	send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		// 	return false;
		// }
		std::cout << client.getNickname() << GRAY << " mode" << std::endl;
	}
	// else if (this->checkChannelMode(reqVec, client)) {
		
	// }
}
