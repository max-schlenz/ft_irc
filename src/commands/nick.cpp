#include "Server.hpp"
# include <cstdlib>

//: oldNick!~UserName@host NICK newNick
void Server::nick(std::vector<std::string> reqVec, Client &client)
{
	if (this->checkNick(reqVec, client))
	{
		std::string oldNick = client.getNickname();
		std::string newNick = reqVec[1];
		std::string addon;
		int i = 2;
		if (this->_clientsM.size() != 0 && this->_clientsM.find(newNick) != this->_clientsM.end()) {
			newNick = newNick + "|" + itos(i);
			std::cout << "client size" << this->_clientsM.size() << std::endl;
		}
		while (this->_clientsM.find(newNick) != this->_clientsM.end()) {
			addon = itos(i);
			std::cout << "size: " << reqVec[1].size() << std::endl;
			std::cout << "new nick size" << newNick.size() << std::endl;
			newNick = newNick.replace(reqVec[1].size() + 1, addon.size(), addon);
			++i;
		}

		client.setNickname(newNick);
		// send(client.getSock(), response.c_str(), response.size(), 0);
		std::string response;

		if (!client.getRegistered())
		{
			std::cout << "if" << std::endl;

			this->sendResponse(client, ":127.0.0.1 001 " + client.getNickname() + " :welcome, " + client.getNickname() + "!" + client.getUsername() + "@" + "127.0.0.1\r\n");
			this->sendMsgToAll(client, ":" + oldNick + "!" + client.getUsername() + "@127.0.0.1 NICK " + reqVec[1] + "\r\n");
			client.setRegistered(true);
			this->_clientsM[client.getNickname()] = &client;
		} else {
			std::cout << "else"<< std::endl;

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
			client.setRegistered(true);
			this->_clientsM[newNick] = &client;
		}
	}
}
