#include "Server.hpp"
# include <cstdlib>
# include <sys/wait.h>

//: oldNick!~UserName@host NICK newNick
void Server::nick(std::vector<std::string> reqVec, Client &client)
{
	if (client.getKick())
		return;
	if (this->checkNick(reqVec, client))
	{
		std::string oldNick = client.getNickname();
		std::string newNick = reqVec[1];
		std::string addon;
		client.setNickname(newNick);
		// send(client.getSock(), response.c_str(), response.size(), 0);
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
		} else {
			// std::cout << "else"<< std::endl;

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
