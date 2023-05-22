#include "Server.hpp"
# include <cstdlib>

//: oldNick!~UserName@host NICK newNick
void Server::nick(std::vector<std::string> reqVec, Client &client)
{
	if (this->checkNick(reqVec, client))
	{
		std::string oldNick = client.getNickname();
		std::string newNick = reqVec[1];
		int i = 2;
		while (this->_clientsM.find(newNick) != this->_clientsM.end()) {
			newNick = newNick + "|" + std::to_string(i);
			++i;
		}

		client.setNickname(newNick);
		// send(client.getSock(), response.c_str(), response.size(), 0);
		std::string response;

		if (!client.getRegistered())
		{
			std::cout << "if" << std::endl;
			response = ":127.0.0.1 001 " + client.getNickname() + " :welcome, " + client.getNickname() + "!" + client.getUsername() + "@" + "127.0.0.1\r\n";
			send(client.getSock(), response.c_str(), response.size(), 0);
			std::string response = ":" + oldNick + "!" + client.getUsername() + "@127.0.0.1 NICK " + reqVec[1] + "\r\n";
			this->sendMsgToAll(client, response);
			client.setRegistered(true);
			this->_clientsM[client.getNickname()] = &client;
		} else {
			std::cout << "else"<< std::endl;
			response = ":" + oldNick + "!" + client.getUsername() + "@127.0.0.1 NICK " + reqVec[1] + "\r\n";
			this->sendMsgToAll(client, response);
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
		std::cout << oldNick << GRAY << " NICK set to " << reqVec[1] << RESET << std::endl;
			// this->_clientsM[oldNick]->setNickname(newNick);
	}
}
