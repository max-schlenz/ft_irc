#include "Server.hpp"
# include <cstdlib>
# include <sys/wait.h>

/*void Server::sendNewNickReq(std::vector<std::string> reqVec, Client& client)
{
	std::string oldNick = client.getNickname();
	std::string reqNick = reqVec[1];
	/*std::string addon;
	int i = 2;
	int pos = reqNick.find("|");*/
	/*if (pos != std::string::npos) {
		reqNick.replace(pos, reqNick.size() - pos, "");
	} else {
		
		reqNick = reqNick + "|" + itos(i);
	}*/
	/*if (pos == std::string::npos) {
		reqNick = reqNick + "|" + itos(i);
	}
	while (this->_clientsM.find(reqNick) != this->_clientsM.end()) {
		addon = itos(i);
		std::cout << "size: " << reqVec[1].size() << std::endl;
		std::cout << "new nick size" << reqNick.size() << std::endl;
		reqNick = reqNick.replace(reqVec[1].size() + 1, addon.size(), addon);
		++i;
	}
	std::string response = E_NICKNAMEINUSE(oldNick, reqNick);
	this->sendResponse(client, response);
	return;
}*/

//: oldNick!~UserName@host NICK newNick
void Server::nick(std::vector<std::string> reqVec, Client &client)
{

	if (client.getKick())
		return;
	if (this->checkNick(reqVec, client))
	{
		std::string oldNick = client.getNickname();
		std::string newNick = reqVec[1];
 		/*if (this->_clientsM.size() != 0 && this->_clientsM.find(newNick) != this->_clientsM.end()) {
			this->sendNewNickReq(reqVec, client);
			return;
		}	*/
		client.setNickname(newNick);
		std::string response;
		if (!client.getNickRegistered())
		{
			if (this->_key_set && !client.getPass())
			{
				std::cout << RED << "No password provided by client!" << RESET << std::endl;
				client.setKick(true);
				return;
			}
			client.setNickRegistered(true);
			this->_clientsM[client.getNickname()] = &client;
			if (!client.getUserRegistered())
				this->welcome(client);
		} else {
			this->sendMsgToAll(client, ":" + oldNick + "!" + client.getUsername() + "@127.0.0.1 NICK " + reqVec[1] + "\r\n");
			for (std::map<std::string, Channel>::iterator it = this->_channelsM.begin(); it != this->_channelsM.end(); ++it) //this is not workin yet
			{
				if (it->second.getClientsM().find(oldNick) != it->second.getClientsM().end()) {
					std::cout << "dellll" << std::endl;
					it->second.getClientsM().erase(oldNick);
					it->second.getClientsM()[newNick] = &client;
				}
			}
			this->_clientsM.erase(oldNick);
			client.setNickRegistered(true);
			this->_clientsM[newNick] = &client;
		}
	}
}
